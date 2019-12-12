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
%{
#include <stdio.h>
#include <pthread.h>
#include "scanner.h"
#include "gdb_protocol.h"
#include "debug.h"

#define YY_NO_INPUT

#define PACKET 0

#define MAX_ERROR_SIZE 1024

typedef struct parser_data {
	gdb_protocol_data_t gdb;
    char buffer[MAX_ERROR_SIZE];
    int len;
} *parser_data_t;

static void handle_error(parser_data_t data) {
    if (data->len) {
        data->buffer[data->len] = '\0';
        debug("gdb protocol error, invalid data : %s, "
              "requiring retransmission\n", data->buffer);
        gdb_require_retransmission(data->gdb);
        data->len = 0;
    }
}

static void add_character_to_error(parser_data_t data, char c) {
    if (data->len > MAX_ERROR_SIZE-2)
        handle_error(data);
    data->buffer[data->len++] = c;
}
%}
%option interactive always-interactive read noyywrap nounput
%option reentrant extra-type="parser_data_t"

HEX	[0-9a-fA-F]
%%
"+"			handle_error(yyextra);debug("Received ack\n");
"-"			{
                        handle_error(yyextra);
                        debug("Received request for retransmission\n");
                        gdb_transmit_packet(yyextra->gdb);
                        }
"$"[^$#]*"#"{HEX}{HEX}	{
						int result;
                        handle_error(yyextra);
						result = gdb_packet_check(yyextra->gdb, yytext, yyleng);
						if (result)
							return PACKET;
                        }
.			add_character_to_error(yyextra, *yytext);
<<EOF>>			handle_error(yyextra);return EOF;
%%
yyscan_t scanner;
struct parser_data data;

void init_gdb_scanner(gdb_protocol_data_t gdb, int in) {
    FILE *f;

	data.gdb = gdb;
    data.len = 0;
    
    f = fdopen(in, "r");
    if (f == NULL) {
        perror("Cannot open input stream from gdb");
        return;
    }
    yylex_init_extra(&data, &scanner);
    yyset_in(f, scanner);
}

int gdb_get_packet(gdb_protocol_data_t gdb) {
    return yylex(scanner);
}

void destroy_gdb_scanner(gdb_protocol_data_t gdb) {
    yylex_destroy(scanner);
}
