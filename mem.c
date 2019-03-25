/**
 * @file: mem.h
 * @desc: Defines the following 8086 instructions:
 *        a) HLT
 *        b) MOV
 *        c) NEG
 *        d) NOP
 */ 

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "mem.h"

/**
 * @desc  : Implements the HLT instruction.
 * @param : glob - unused
 *          buf  - unused
 *          size - unused
 * @return: int  - 0 if fail, 1 if success.
 */ 
int hlt(glob_t *glob, char *buf, unsigned long size) {
	/* tengine intercepts this return value and sends to main. */
	return 1;
}

/**
 * @desc  : Implements the MOV instruction.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: int  - 0 if fail, 1 if success.
 */ 
int move(glob_t *glob, char *buf, unsigned long size) {
	assert(glob && glob->registers);

	char *src  = glob->tokens[2];
	char *dest = glob->tokens[1];

	if (!is_valid_op(dest) || !is_valid_op(src)) {
		fprintf(stderr, "Invalid operands for MOV: [%s] [%s]\n", dest, src);
		return 0;
	}

	if (!is_loc_reg(dest)) {
		fprintf(stderr, "TErr: Dest must be register.\n");
		return 0;
	}

	char temp[1024] = { 0 };
	char *back = &src[strlen(src) - 1];

	/* Immediate addressing mode. */
	if (*back == HEX_FS) {
		switch (get_reg_size(dest)) {
		case 8: {
			if (strlen(src) > 3) {
				fprintf(stderr, "Data too large for 8 bit reg [%s].\n", dest);
				return 0;
			}

			break;
		}

		case 16: {
			if (strlen(src) > 5) {
				fprintf(stderr, "Data too large for 16 bit reg [%s].\n", dest);
				return 0;
			}

			break;
		}
		}

		/* Clip HEX_FS */
		*back = '\0';
		for (int i = 0; i < strlen(src); i++) {
			if (isalpha(src[i])) {
				if (src[i] > 'F') {
					fprintf(stderr, "Invalid char in hex [%c].\n", src[i]);
					return 0;
				}
			}
		}

		memcpy(temp, src, sizeof(temp));
	/* Register addressing mode. */
	} else if (is_loc_reg(src)) {
		int r1_sz = get_reg_size(src);
		int r2_sz = get_reg_size(dest);

		if (r1_sz != r2_sz) {
			fprintf(stderr, "MOV: Both registers must be of same size.\n");
			return 0;
		}

		if (!get_reg_val(glob, src, temp, sizeof(temp))) {
			fprintf(stderr, "Could not fetch reg [%s] value.\n", src);
			return 0;
		}
	} else {
		fprintf(stderr, "MOV: Invalid data specified.\n");
		return 0;
	}

	if (strlen(temp) == 0) {
		fprintf(stderr, "Warning: Setting empty value. [%s] -> [%s]\n",
		        temp, dest);
	}

	/* Set the destination register value. */
	if (!set_reg_val(glob, dest, temp)) {
		fprintf(stderr, "Could not set reg [%s] value.\n", dest);
		return 0;
	}

	return 1;
}

/**
 * @desc  : Implements the NEG instruction.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: int  - 0 if fail, 1 if success.
 */  
int neg(glob_t *glob, char *buf, unsigned long size) {
	assert(glob && glob->registers);

	char *op  = glob->tokens[1];
	char *ptr = get_reg_ptr(glob, op);

	if (!ptr) {
		fprintf(stderr, "Invalid flag specified [%s].\n", op);
		return 0;
	}

	int x = *ptr == '-' ? -1 : 1;
	char *dup = *ptr == '-' ? ptr + 1 : ptr + strlen(ptr) - 1;

	while (*dup) {
		*(dup + x) = *dup;
		dup += (x * -1);
	}

	switch (x) {
	case 1 : *ptr = '-'; break;
	default: *(ptr + strlen(ptr) - 1) = '\0'; break;
	}

	return 1;
}

/**
 * @desc  : Implements the NOP instruction.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: int  - 0 if fail, 1 if success.
 */  
int nop(glob_t *glob, char *buf, unsigned long size) {
	return 1;
}
