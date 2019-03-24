/**
 * @file: flags.h
 * @desc: Defines the following 8086 instructions:
 *        a) CLC
 *        b) CLD
 *        c) CLI
 *        d) CMC
 *        d) STC
 *        e) STD
 *        f) STI
 */ 

#ifndef _ASE_FLAGS_H_
#define _ASE_FLAGS_H_

#include "glob.h"

int cmc          (glob_t *glob, char *buf, unsigned long size);
int clear_flag   (glob_t *glob, char *buf, unsigned long size);
int get_flag_val (glob_t *glob, char *flag);
int set_flag     (glob_t *glob, char *buf, unsigned long size);

#endif