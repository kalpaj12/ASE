/**
 * @file: mem.h
 * @desc: Defines the following 8086 instructions:
 *        a) HLT
 *        b) MOV
 *        c) NEG
 *        d) NOP
 *        e) XCHG
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
	return -1;
}

/**
 * @desc  : Implements the MOV instruction.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: int  - 0 if fail, 1 if success.
 */ 
int move(glob_t *glob, char *buf, unsigned long size) {

	char val[1024];
	char *src  = glob->tokens[2];
	char *dest = glob->tokens[1];

	if (!is_loc_reg(dest)) {
		fprintf(stderr, "MOV: Destination must be a register.\n");
		return 0;
	}

	char *last = &src[strlen(src) - 1];
	if (!isdigit(*last)) {
		*last = toupper(*last);
	}

	if (is_loc_reg(src)) {
		int dest_sz = get_reg_size(dest);
		int src_sz  = get_reg_size(src);

		if (dest_sz != src_sz) {
			fprintf(stderr, "MOV: Both registers must be of same size.\n");
			return 0;
		}

		get_reg_val(glob, src, val, sizeof(val));
		goto set;
	}

	switch (*last) {
	case 'H': {
		*last = '\0';
		if (!is_valid_hex(src)) {
			fprintf(stderr, "MOV: Invalid value [%s].\n", src);
			return 0;	
		}

		strcpy(val, src);
		break;
	}

	default: {
		if (isalpha(*last)) {
			fprintf(stderr, "MOV: Invalid specifier [%c].\n", *last);
			return 0;
		}

		int k_len = strlen(src);
		if (k_len > ((get_reg_size(dest) == 16) ? 4 : 2)) {
			fprintf(stderr, "MOV: Operand too large for %s.\n", dest);
			return 0;
		}

		char *ptr = src;
		while (*ptr) {
			if (!isalnum(*ptr++)) {
				fprintf(stderr, "MOV: Invalid value [%s].\n", src);
				return 0;
			}
		}

		sprintf(val, "%x", (unsigned int)strtol(src, NULL, 0));
		break;
	}
	}

	set:
	if (!set_reg_val(glob, dest, val)) {
		fprintf(stderr, "MOV: Could not set value.\n");
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

/**
 * @desc  : Implements the XCHG instruction.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: int  - 0 if fail, 1 if success.
 * 
 */
int xchg(glob_t *glob, char *buf, unsigned long size) {

	char *d_ptr = get_reg_ptr(glob, glob->tokens[1]);
	char *s_ptr = get_reg_ptr(glob, glob->tokens[2]);

	if (d_ptr && s_ptr) {
		char *t = malloc(strlen(d_ptr) + 1);
		if (!t) {
			return 0;
		}

		strcpy(t, d_ptr);
		strcpy(d_ptr, s_ptr);
		strcpy(s_ptr, t);
		free(t);
		return 1;
	}

	return 0;
}
