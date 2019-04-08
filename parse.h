/**
 * @file: parse.c
 * @desc: Declares functions that help parse source lines and declares
 *        the following 8086 instructions:
 *        a) JC
 *        b) JE
 *        c) JP
 *        d) JNC
 *        e) JNE
 *        f) JNP
 *        g) JMP
 *        h) JCXZ
 */ 

#ifndef _ASE_PARSE_
#define _ASE_PARSE_

#include <stdio.h>
#include "glob.h"

#define HEX_FS  'H'

int  fetch_dest     (char *line, char *buf, unsigned long size);
int  fetch_instr    (char *line, char *buf, unsigned long size);
int  fetch_src      (char *line, char *buf, unsigned long size);
int  get_reg_size   (char *reg);
int  is_instr_valid (char *instr);
int  is_loc_reg     (char *loc);
int  is_loc_addr    (char *loc);
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