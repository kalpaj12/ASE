/**
 * @file: mem.h
 * @desc: Declares the following 8086 instructions:
 *        a) HLT
 *        b) MOV
 *        c) NEG
 *        d) NOP
 */

#ifndef _ASE_MEM_H_
#define _ASE_MEM_H_

#include "glob.h"
#include "parse.h"

int hlt  (glob_t *glob, char *buf, unsigned long size);
int move (glob_t *glob, char *buf, unsigned long size);
int neg  (glob_t *glob, char *buf, unsigned long size);
int nop  (glob_t *glob, char *buf, unsigned long size);

#endif