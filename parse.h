/**
 * @file: parse.h
 * @desc: Defines functions that help parse source lines and defines
 *        the following 8086 instructions:
 *        JC, JE, JP, JNC, JNE, JNP, JMP, JCXZ
 */ 

#ifndef _ASE_PARSE_
#define _ASE_PARSE_

#include <stdio.h>
#include "glob.h"

#define BIN_FS  'B'
#define HEX_FS  'H'

void binary_repr    (int x, char *buf, unsigned long size);
int  get_reg_size   (char *reg);
int  is_op_reg      (char *op);
int  is_op_addr     (char *op);
int  is_valid_hex   (char *hex);
int  is_valid_op    (char *op);
int  jump           (glob_t *glob, char *buf, unsigned long size);
int  jump_cx        (glob_t *glob, char *buf, unsigned long size);
int  jump_jx        (glob_t *glob, char *buf, unsigned long size);
int  jump_jnx       (glob_t *glob, char *buf, unsigned long size);
int  parse_line     (glob_t *glob, char *line);
int  should_skip_ln (char *line);
int  step_back      (glob_t *glob);

#endif