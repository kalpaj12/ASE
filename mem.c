/**
 * @file: mem.h
 * @desc: Defines the following 8086 instructions:
 *        DEC, HLT, INC, MOV, NEG, NOP, XCHG
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

	/* Strict size checking */
	if (is_op_reg(dest) && is_op_reg(src_)) {
		if (get_reg_size(dest) != get_reg_size(src_)) {
			fprintf(stderr, "move(): both registers must be of same size.\n");
			return 0;
		}
	}

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
		dest = add_to_mem(glob, glob->mem->ds, offset)->val;
	}

	if (is_op_reg(dest)) {
		dest = get_reg_ptr(glob, dest);
	}

	return get_op_val(glob, src_, dest, BUF_SZ);
}

/**
 * @desc  : Implements the NEG instruction.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: int  - 0 if fail, 1 if success.
 */  
int neg(glob_t *glob, char *buf, unsigned long size) {
	if (!glob) {
		fprintf(stderr, "neg(): glob - nullptr.\n");
		return 0;
	}

	assert(glob->n_op == 1);
	char *op = glob->tokens[1], *ptr = NULL;

	if (is_op_addr(op)) {
		char addr[BUF_SZ];
		memcpy(addr, &op[1], BUF_SZ);
		int offset = (int)strtol(addr, NULL, 0);

		mem_nodes_t *node = get_mem_node(glob, offset);
		if (node) {
			ptr = node->val;
		}
	}

	if (is_op_reg(op)) {
		ptr = get_reg_ptr(glob, op);
	}

	if (!ptr) {
		fprintf(stderr, "neg(): Invalid operand specified [%s].\n", op);
		return 0;
	}

	int temp = (int)strtol(ptr, NULL, 16) * -1;
	sprintf(ptr, "%x", temp);
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
 * 
 * @desc  : Implements the INC and DEC instructions.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: int  - 0 if fail, 1 if success.
 */
int unary(glob_t *glob, char *buf, unsigned long size) {
	if (!glob) {
		fprintf(stderr, "unary(): glob - nullptr.\n");
		return 0;
	}

	assert(glob->n_op == 1);
	char *ptr = NULL;
	char *op = glob->tokens[1];
	int uop = strcmp(glob->tokens[0], "INC") == 0 ? 1 : -1;

	if (is_op_addr(op)) {
		char addr[BUF_SZ];
		memcpy(addr, &op[1], strlen(op) - 2);
		int offset = (int)strtol(addr, NULL, 0);

		mem_nodes_t *node = get_mem_node(glob, offset);
		if (node) {
			ptr = node->val;
		}
	}

	if (is_op_reg(op) && !ptr) {
		ptr = get_reg_ptr(glob, op);
	}

	if (!ptr) {
		fprintf(stderr, "unary(): Invalid operand specified [%s].\n", op);
		return 0;
	}

	char val[BUF_SZ];
	memcpy(val, ptr, BUF_SZ);
	sprintf(ptr, "%x", (int)strtol(val, 0, 16) + uop);
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

	assert(glob->n_op == 2);
	char *dest = glob->tokens[1];
	char *src  = glob->tokens[2];

	if (is_op_addr(src) && is_op_addr(dest)) {
		fprintf(stderr, "xchg(): Both the operands cannot be memory addresses.\n");
		return 0;
	}

	if (is_op_reg(dest)) {
		dest = get_reg_ptr(glob, dest);
	}

	if (is_op_reg(src)) {
		src = get_reg_ptr(glob, src);
	}

	int offset = -1;
	if (is_op_addr(dest)) {
		char addr[BUF_SZ];
		memcpy(addr, &dest[1], strlen(dest) - 2);
		offset = (int)strtol(addr, NULL, 0);
	}

	if (is_op_addr(src)) {
		char addr[BUF_SZ];
		memcpy(addr, &src[1], strlen(src) - 2);
		offset = (int)strtol(addr, NULL, 0);
	}

	if (offset != -1) {
		mem_nodes_t *node = get_mem_node(glob, offset);
		if (node) {
			dest = node->val;
		} else {
			
			if (!glob->mem->warned && !getenv("DIW")) {
				fprintf(stderr, "xchg(): Using uninitialised memory location [%d:%d]\n",
				glob->mem->ds,
				offset);
			}

			mem_nodes_t *node = add_to_mem(glob, glob->mem->ds, offset);
			node->seg = glob->mem->ds;
			node->offset = offset;
			node->addr = (node->seg * 10) + node->offset;
			strcpy(node->val, "0");

			dest = node->val;
		}
	}

	char temp[BUF_SZ];
	memcpy(temp, dest, BUF_SZ);
	memcpy(dest, src, BUF_SZ);
	memcpy(src, temp, BUF_SZ);

	return 1;
}
