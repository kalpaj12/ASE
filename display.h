/**
 * @file: display.h
 * @desc: Declares functions that put data from memory, registers and
 *        stack onto the display.
 */ 

#ifndef _ASE_DISPLAY_H_
#define _ASE_DISPLAY_H_

#include "glob.h"
#include "parse.h"

typedef struct args_ {
	int a, f, h, l, m, r, s, v;
} args_t;

void display    (glob_t *glob, args_t p_args);
void show_flags (void);

#endif