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
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "arm_simulator_interface.h"

void usage(char *name) {
	fprintf(stderr, "Usage:\n"
		"%s [ --help ] [ --host hostname ] [ --service servicename ] [ --debug file ] file\n\n"
		"Loads a sample ARM code to a remote simulator. The --debug flag enables the output produced by "
		"calls to the debug function in the named source file.\n" 
		, name);
}

void sample_run(char *hostname, char *servicename) {
	uint32_t address = 0x58;
	// Sample loop written assuming little endian host
	// interacting with a big endian simulator
	uint32_t code[] = {
		0x0500a0e3, // mov r0, #5
		0x010050e2, // subs r0, r0, #1
		0xfdffff1a, // bne fffffd
		0x563412ef  // swi 0x123456
	};
	size_t size = 16;

	arm_simulator_data_t simulator;

	debug("Connecting to simulator using host %s and service %s\n", hostname, servicename);
	simulator = arm_connect(hostname, servicename);
	debug("Fetching code to simulator and setting PC\n");
	arm_write_memory(simulator, address, code, size);
	arm_write_register(simulator, 15, address);
	debug("Running simulator\n");
	arm_run(simulator);
	debug("End of simulation\n");
}

int main(int argc, char *argv[]) {
	int opt;
	char *hostname, *servicename;

	struct option longopts[] = {
		{ "debug", required_argument, NULL, 'd' },
		{ "host", required_argument, NULL, 'H' },
		{ "service", required_argument, NULL, 'S' },
		{ "help", no_argument, NULL, 'h' },
		{ NULL, 0, NULL, 0 }
	};

	hostname = NULL;
	servicename = NULL;
	while ((opt = getopt_long(argc, argv, "S:H:d:h", longopts, NULL)) != -1) {
		switch(opt) {
		case 'H':
			hostname = optarg;
			break;
		case 'S':
			servicename = optarg;
			break;
		case 'h':
			usage(argv[0]);
			exit(0);
		case 'd':
			add_debug_to(optarg);
			break;
		default:
			fprintf(stderr, "Unrecognized option %c\n", opt);
			usage(argv[0]);
			exit(1);
		}
	}

	sample_run(hostname, servicename);
	return 0;
}
