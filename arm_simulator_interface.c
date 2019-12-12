/*
ELF Loader - chargeur/implanteur d'exécutables au format ELF à but pédagogique
Copyright (C) 2012 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/
#include "arm_simulator_interface.h"
#include "csapp.h"
#include "gdb_protocol.h"
#include "debug.h"
#include "scanner.h"

struct arm_simulator_data {
	gdb_protocol_data_t gdb;
};

arm_simulator_data_t arm_connect(char *hostname, char *service) {
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd;

	arm_simulator_data_t arm;

	/* Obtain address(es) matching host/port */

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;	/* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = 0;
	hints.ai_protocol = 0;		  /* Any protocol */

	Getaddrinfo(hostname, service, &hints, &result);

	/* getaddrinfo() returns a list of address structures.
	   Try each address until we successfully connect(2).
	   If socket(2) (or connect(2)) fails, we (close the socket
	   and) try the next address. */

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;		   /* Success */

		close(sfd);
	}

	if (rp == NULL) 		/* No address succeeded */
		error("Could not connect\n");

	freeaddrinfo(result);	/* No longer needed */

	arm = malloc(sizeof(struct arm_simulator_data));
	if (arm == NULL) 
		error("Memory allocation error\n");

	arm->gdb = gdb_init_data(sfd);
	init_gdb_scanner(arm->gdb, sfd);
	return arm;
}

void arm_write_register(arm_simulator_data_t simulator, uint8_t num, uint32_t value) {
	gdb_write_register(simulator->gdb, num, value);
}

void arm_write_memory(arm_simulator_data_t simulator, uint32_t address, void *buffer, size_t size) {
	gdb_write_memory(simulator->gdb, address, buffer, size);
}

static void cleanup(arm_simulator_data_t simulator) {
	destroy_gdb_scanner(simulator->gdb);
	gdb_destroy_data(simulator->gdb);
	free(simulator);
}

void arm_run(arm_simulator_data_t simulator) {
	gdb_cont(simulator->gdb);
	cleanup(simulator);
}

int arm_step(arm_simulator_data_t simulator) {
	int answer;
	
	answer = gdb_step(simulator->gdb);
	if (answer == EOF)
		cleanup(simulator);
	return answer;
}
