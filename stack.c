/**
 * @file: stack.c
 * @desc: Defines the following 8086 instructions:
 *        a) PUSH
 *        b) PEEK
 *        c) POP
 */

#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

/**
 * @desc  : Implements 8086 POP function.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: 0 if fail, 1 if success.
 */ 
int pop(glob_t *glob, char *buf, unsigned long size) {
	if (!glob || !glob->stack) {
		fprintf(stderr, "pop(): args - nullptrs.\n");
		return 0;
	}

	char *op = glob->tokens[1];
	char *dest = NULL;

	if (is_op_addr(op)) {
		char addr[BUF_SZ];
		memset(addr, 0, sizeof(addr));
		memcpy(addr, &op[1], strlen(op) - 2);

		for (int i = 0; i < strlen(addr); i++) {
			if (!isdigit(addr[i])) {
				fprintf(stderr, "pop(): invalid address [%s].\n", addr);
				return 0;
			}
		}

		int offset = (int)strtol(addr, NULL, 0);
		dest = add_to_mem(glob, 0, offset)->val;
		
	} else if (is_op_reg(op)) {
		dest = get_reg_ptr(glob, op);
	}

	assert(dest);
	int idx = glob->stack->top--;
	if (idx == -1) {
		fprintf(stderr, "Illegal instruction: POP before PUSH.\n");
		return 0;
	}
	
	char *s_ptr = glob->stack->arr[idx];
	int ret = get_op_val(glob, s_ptr, dest, -1);
	free(glob->stack->arr[idx]);

	return ret;
}

/**
 * @desc  : Implements 8086 PUSH function.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: 0 if fail, 1 if success.
 */ 
int push(glob_t *glob, char *buf, unsigned long size) {
	if (!glob || !glob->stack) {
		fprintf(stderr, "push(): nullptr - cannot push.\n");
		return 0;
	}

	int idx = ++glob->stack->top;
	char *op = glob->tokens[1];
	char **s_ptr = &glob->stack->arr[idx];

	if (!s_ptr) {
		return 0;
	}

	if (!*s_ptr) {
		*s_ptr = malloc((unsigned long)BUF_SZ);
	}

	return get_op_val(glob, op, *s_ptr, size);
}