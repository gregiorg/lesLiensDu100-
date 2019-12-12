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
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "gdb_protocol.h"
#include "scanner.h"
#include "debug.h"
#include "csapp.h"
#include "util.h"

#define MAX_PACKET_SIZE 1024
#define MAX_BUFFER_SIZE (MAX_PACKET_SIZE-6)

struct gdb_protocol_data {
    int fd;
    char packet[MAX_PACKET_SIZE];
    int len;
    char *buffer;
	char *answer;
	int answer_len;
};

static void gdb_send_ack(gdb_protocol_data_t gdb) {
    Rio_writen(gdb->fd, "+", 1);
}

static void gdb_send_buffer(gdb_protocol_data_t gdb) {
    unsigned char check=0;
    int i;

    gdb->packet[0] = '$';
    i = 1;
    while (gdb->len--) {
        check += gdb->packet[i];
        i++;
    }
    gdb->packet[i++] = '#';
    sprintf(gdb->packet+i, "%02x", check);
    i += 2;
    gdb->packet[i] = '\0';
    gdb->len = i;
    gdb_transmit_packet(gdb);
}

static void gdb_send_data(gdb_protocol_data_t gdb, char *data) {
    strcpy(gdb->buffer, data);
	gdb->len = strlen(data);
    gdb_send_buffer(gdb);
}

static int gdb_get_answer(gdb_protocol_data_t gdb) {
	int answer;

	answer = gdb_get_packet(gdb);
	// At this time, we don't have any good interpretation of the answer, just trace
	// it as a debugging information
	if (answer != EOF)
		debug("Simulator answered: %s\n", gdb->answer);
	else
		debug("Simulator exited\n");
	return answer;
}

/* Read and write to/from a string of bytes (in hexadecimal) in local byte
 * order */
static void write_uint32(char *data, uint32_t value) {
    int i, step;
    int target_big_endian;
    union {
        unsigned char bytes[4];
        uint32_t integer;
    } mem;

    mem.integer = value;
    #ifdef BIG_ENDIAN_SIMULATOR
        target_big_endian = 1;
    #else
        target_big_endian = 0;
    #endif

    if (is_big_endian() == target_big_endian) {
        i = 0;
        step = 1;
    } else {
        i = 3;
        step = -1;
    }

    for (; (i<4) && (i>=0); i+=step) {
        sprintf(data, "%02x", mem.bytes[i]);
        data += 2;
    }
    *data = '\0';
}

/* Publicly accessible functions */
gdb_protocol_data_t gdb_init_data(int fd) {
    gdb_protocol_data_t gdb;

    gdb = malloc(sizeof(struct gdb_protocol_data));
    if (gdb) {
        gdb->fd = fd;
        gdb->len = 0;
        gdb->buffer = gdb->packet+1;
    }
    return gdb;
}

void gdb_destroy_data(gdb_protocol_data_t gdb) {
	free(gdb);
}

void gdb_write_register(gdb_protocol_data_t gdb, uint8_t num, uint32_t value) {
	char *buffer = gdb->buffer;

	sprintf(buffer, "P%02x=", num);
	write_uint32(buffer+4, value);
	gdb->len = 12;
	gdb_send_buffer(gdb);
	gdb_get_answer(gdb);
}

void gdb_write_memory(gdb_protocol_data_t gdb, uint32_t address, void *buffer, size_t size) {
	char *dest_buffer, *src_buffer;
	int bytes_copied, buffer_size;

	src_buffer = buffer;
	while (size > 0) {
		dest_buffer = gdb->buffer+19;
		bytes_copied = 0;
		buffer_size = 0;

		while ((buffer_size < MAX_BUFFER_SIZE-1) &&
				(bytes_copied < size)) {
			switch (*src_buffer) {
			case '$':
			case '#':
			case 0x7d:
				*(dest_buffer++) = (char) 0x7d;
				*dest_buffer = *src_buffer ^ (char) 0x20;
				buffer_size++;
				break;
			default:
				*dest_buffer = *src_buffer;
			}
			buffer_size++;
			bytes_copied++;
			dest_buffer++;
			src_buffer++;
		}

		sprintf(gdb->buffer, "X%08x,%08x", address, bytes_copied);
		gdb->buffer[18] = ':';
		gdb->len = 19+buffer_size;
		gdb_send_buffer(gdb);
		gdb_get_answer(gdb);
		address += bytes_copied;
		size -= bytes_copied;
	}
}

void gdb_cont(gdb_protocol_data_t gdb) {
	int answer;

	gdb_send_data(gdb, "c");
	answer = gdb_get_answer(gdb);
	if (answer != EOF)
		debug("Unexpected packet after the end of the simulation: %s\n", gdb->packet);
}

int gdb_step(gdb_protocol_data_t gdb) {
	int answer;

	gdb_send_data(gdb, "s");
	answer = gdb_get_answer(gdb);
	return answer;
}

void gdb_require_retransmission(gdb_protocol_data_t gdb) {
    Rio_writen(gdb->fd, "-", 1);
}

int gdb_packet_check(gdb_protocol_data_t gdb, char *packet, int length) {
    int i;
    unsigned char check=0;
    unsigned int given;

    for (i=1; i<length-3; i++)
        check += packet[i];
    sscanf(packet+i+1, "%x", &given);
    debug("Received packet : ");
    debug_raw_binary(packet, min(16, strlen(packet)));
    if (check == given) {
        debug_raw(", checksum ok\n");
        gdb_send_ack(gdb);
    } else {
        debug_raw(", checksum failed, expected %02x got %02x\n", given, check);
        debug("Requiring retransmission\n");
        gdb_require_retransmission(gdb);
        return 0;
    }
	gdb->answer = packet;
	gdb->answer_len = length;
	return 1;
}

void gdb_transmit_packet(gdb_protocol_data_t gdb) {
    debug("Transmitting packet: %s\n", gdb->packet);
    Rio_writen(gdb->fd, gdb->packet, gdb->len);
}
