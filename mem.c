/**
 * @file: mem.h
 * @desc: Defines the following 8086 instructions:
 *        LOC_TYPE) HLT
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
	if (!glob) {
		fprintf(stderr, "move(): glob - nullptr.\n");
		return 1;
	}

	if (glob->n_op != 2) {
		char *n_op = glob->n_op > 2 ? "excess" : "missing";
		fprintf(stderr, "move(): %s operands.\n", n_op);
		return 0;
	}

	char *dest = glob->tokens[1];
	char *src_ = glob->tokens[2];

	if (!is_valid_op(dest)) {
		fprintf(stderr, "move(): invalid destination operand.\n");
		return 0;
	}

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
		dest = add_to_mem(glob, 0, offset)->val;
	}

	if (is_op_reg(dest)) {
		dest = get_reg_ptr(glob, dest);
	}

	return get_op_val(glob, src_, dest, REG_BUF);
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
	if (!glob) {
		fprintf(stderr, "xchg(): glob - nullptr.\n");
		return 0;
	}

	char *dest = get_reg_ptr(glob, glob->tokens[1]);
	char *src_ = get_reg_ptr(glob, glob->tokens[2]);

	if (!dest || !src_) {
		return 0;
	}
	
	char *temp = malloc(strlen(dest) + 1);
	if (!temp) {
		return 0;
	}

	strcpy(temp, dest);
	strcpy(dest, src_);
	strcpy(src_, temp);
	free(temp);
	return 1;
}
