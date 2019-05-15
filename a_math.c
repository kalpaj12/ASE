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
		fprintf(stderr, "math_op(): glob - null\n");
		return 0;
	}

	/* Let AX (accumulator) be the default destination */
	char *ptr = get_reg_ptr(glob, REG_AX);

	char *inst = glob->tokens[0];
	char *dest = glob->tokens[1];
	char *src_ = glob->tokens[2];
	char dval[BUF_SZ], sval[BUF_SZ], res[BUF_SZ];

	if (strcmp(inst, DIV) == 0 || strcmp(inst, MUL) == 0) {
		if (glob->n_op != 1) {
			char *z = glob->n_op > 1 ? "excess" : "missing";
			fprintf(stderr, "math_op(): %s - %s instructions.\n", inst, z);
			return 0;
		}

		/**
		 * DIV & MUL take only 1 operand.
		 * The default and the destination operand is AX (accumulator).
		 */
		dest = REG_AX;
		src_ = glob->tokens[1];
	}

	int ret1 = get_op_val(glob, dest, dval, sizeof(dval));
	int ret2 = get_op_val(glob, src_, sval, sizeof(sval));

	if (ret1 ^ ret2) {
		return 0;
	}

	int c_dval = (int)strtol(dval, NULL, 16);
	int c_sval = (int)strtol(sval, NULL, 16);

	if (strcmp(inst, ADD) == 0) {
		sprintf(res, "%d", c_dval + c_sval);
		if (strlen(res) > 4) {
			strcpy(res, "0");
			glob->flags->of = 1;
			return 1;
		}
	} else if (strcmp(inst, SUB) == 0) {
		/* Answer is 0. Set zero flag */
		if (c_dval == c_sval) {
			glob->flags->zf = 1;
		}

		sprintf(res, "%d", c_dval - c_sval);
	} else if (strcmp(inst, MUL) == 0) {
		sprintf(res, "%d", c_dval * c_sval);
		goto set;
	} else if (strcmp(inst, DIV) == 0) {
		sprintf(res, "%d", c_dval / c_sval);
		goto set;
	} else if (strcmp(inst, CMP) == 0) {
		if (c_dval < c_sval) {
			glob->flags->cf = 1;
		} else if (c_dval == c_sval) {
			glob->flags->zf = 1;
		}
		return 1;
	} else {
		return 0;
	}

	/**
	 * Set the destination pointer. 
	 * This part is skipped if the default destination has been set to
	 * AX (accumulator).
	 */
	if (is_op_addr(dest)) {
		char addr[BUF_SZ];
		memset(addr, 0, sizeof(addr));
		memcpy(addr, &dest[1], strlen(dest) - 2);
		
		for (int i = 0; i < strlen(addr); i++) {
			if (!isdigit(addr[i])) {
				fprintf(stderr, "move(): invalid address [%s].\n", addr);
				return 0;
			}
		}

		int offset = (int)strtol(addr, NULL, 0);
		ptr = add_to_mem(glob, 0, offset)->val;
	} else if (is_op_reg(dest)) {
		ptr = get_reg_ptr(glob, dest);
	}

	/**
	 * We skip to this part if AX (accumulator) has been decided as the
	 * default destination.
	 */
	set:
	assert(ptr);

	return get_op_val(glob, res, ptr, BUF_SZ);
}