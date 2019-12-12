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
#ifndef __ARM_SIMULATOR_INTERFACE__
#define __ARM_SIMULATOR_INTERFACE__
#include <stdint.h>
#include <sys/types.h>

typedef struct arm_simulator_data *arm_simulator_data_t;

arm_simulator_data_t arm_connect(char *hostname, char *service);
void arm_write_register(arm_simulator_data_t simulator, uint8_t num, uint32_t value);
void arm_write_memory(arm_simulator_data_t simulator, uint32_t address, void *buffer, size_t size);
void arm_run(arm_simulator_data_t simulator);
int arm_step(arm_simulator_data_t simulator);

#endif
