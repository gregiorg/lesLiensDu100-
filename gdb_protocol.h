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
#ifndef __GDB_PROTOCOL_H__
#define __GDB_PROTOCOL_H__
#include <stdint.h>

typedef struct gdb_protocol_data *gdb_protocol_data_t;

gdb_protocol_data_t gdb_init_data(int fd);
void gdb_destroy_data(gdb_protocol_data_t gdb);
void gdb_write_register(gdb_protocol_data_t gdb, uint8_t num, uint32_t value);
void gdb_write_memory(gdb_protocol_data_t gdb, uint32_t address, void *buffer, size_t size);
void gdb_cont(gdb_protocol_data_t gdb);
int gdb_step(gdb_protocol_data_t gdb);
void gdb_require_retransmission(gdb_protocol_data_t gdb);
int gdb_packet_check(gdb_protocol_data_t gdb, char *packet, int length);
void gdb_transmit_packet(gdb_protocol_data_t gdb);

#endif
