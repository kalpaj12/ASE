/**
 * @file: stack.c
 * @desc: Declares the following 8086 instructions:
 *        a) PUSH
 *        b) PEEK
 *        c) POP
 */

#ifndef _ASE_STACK_H_
#define _ASE_STACK_H_

#include "glob.h"
#include "parse.h"

int pop  (glob_t *glob, char *buf, unsigned long size);
int push (glob_t *glob, char *buf, unsigned long size);

#endif
