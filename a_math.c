/**
 * @file: a_math.c
 * @desc: Defines the following 8086 instructions:
 *        a) ADD
 *        b) SUB
 *        c) MUL
 *        d) DIV
 *        e) CMP
 */

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "a_math.h"

/**
 * @desc  : Implements the ADD instruction.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: int  - 0 if fail, 1 if success.
 */ 
int math_op(glob_t *glob, char *buf, unsigned long size) {

	if (!glob) {
		fprintf(stderr, "add(): glob - null\n");
		return 0;
	}

	char val[1024];
	char *instr = glob->tokens[0];
	char *dest  = glob->tokens[1];
	char *src   = glob->tokens[2];

	/* Accumulator is the default register for MUL & DIV */
	if (!strcmp(instr, MUL) || !strcmp(instr, DIV)) {
		dest = get_reg_ptr(glob, REG_AX);
	}

	/* For now */
	assert(is_loc_reg(dest));
	unsigned long d_val = 0, s_val = 0, result = 0;

	if (!reg_to_ul(glob, dest, &d_val)) {
		fprintf(stderr, "add(): Could not fetch dest value.\n");
		return 0;
	}

	if (is_loc_reg(src)) {
		(void)reg_to_ul(glob, src, &s_val);
		goto set;
	}

	/* Immediate addressing mode */
	int base = 0;
	char *last = &src[strlen(src) - 1];

	switch (*last) {
	case HEX_FS: {
		*last = '\0';
		if (!is_valid_hex(src)) {
			fprintf(stderr, "add(): [%s] is not valid hex.\n", src);
			return 0;
		}

		base = 16;
		break;
	}

	default: {
		char *ptr = src;
		while (*ptr) {
			if (!isdigit(*ptr++)) {
				fprintf(stderr, "add(): [%s] is invalid value.\n", src);
				return 0;
			}
		}

		base = 0;
		break;			
	}
	}

	s_val = strtol(src, NULL, base);

	set:
	/**
	 * The code till previous line is common for many instructions.
	 * The next section here identifies the instruction that was called.
	 */
	if (!strcmp(instr, ADD)) {
		result = d_val + s_val;
	} else if (!strcmp(instr, CMP)) {
		if (d_val == s_val) {
			glob->flags->zf = 1;
		} else if (d_val < s_val) {
			glob->flags->cf = 1;
		}

		return 1;
	} else {
		fprintf(stderr, "math_op(): Unsupported operand [%s].\n", instr);
		return 0;
	}

	sprintf(val, "%ld", result);
	(void)set_reg_val(glob, dest, val, 1);
	return 1;
}