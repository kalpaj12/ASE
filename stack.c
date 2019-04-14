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
 * @desc  : Implements 8086 PUSH function.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: 0 if fail, 1 if success.
 */ 
int push(glob_t *glob, char *buf, unsigned long size) {
	if (!glob || !glob->stack) {
		return 0;
	}

	int idx = ++glob->stack->top;
	char **s_ptr = &glob->stack->arr[idx];

	if (!s_ptr) {
		return 0;
	}

	if (!*s_ptr) {
		*s_ptr = malloc((unsigned long)128);
	}

	char *ptr  = NULL;
	char *reg  = glob->tokens[1];
	char *back = &reg[strlen(reg) - 1];

	/* Immediate addressing mode. */
	if (*back == HEX_FS) {
		*back = '\0';
		ptr = reg;
	} else {
		/* Register addressing mode. */
		ptr = get_reg_ptr(glob, reg);
	}

	if (!ptr) {
		char *x = reg;
		while (*x) {
			if (!isdigit(*x++)) {
				fprintf(stderr, "push(): Invalid value [%s].\n", reg);
				return 0;
			}
		}

		if (strlen(reg) > 4) {
			fprintf(stderr, "push(): Operand too large [%s]\n", reg);
			return 0;
		}

		sprintf(*s_ptr, "%x", (unsigned int)strtol(reg, 0, 0));
		return 1;
	}
	
	memcpy(*s_ptr, ptr, (unsigned long)128);
	return 1;
}

/**
 * @desc  : Fetches the element on the stack.
 * @param : glob -
 *          buf  - buffer that receives the value from stack.
 *          size - size of receiving buffer.
 * @return: 0 if fail, 1 if success.
 */ 
int peek(glob_t *glob, char *buf, unsigned long size) {
	return -1;
}


/**
 * @desc  : Implements 8086 POP function.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: 0 if fail, 1 if success.
 */ 
int pop(glob_t *glob, char *buf, unsigned long size) {
	assert(glob && glob->stack);

	char *reg = glob->tokens[1];
	char *ptr = get_reg_ptr(glob, reg);

	int idx = glob->stack->top--;
	if (idx == -1) {
		fprintf(stderr, "Illegal instruction: PUSH before POP.\n");
		return 0;
	}
	
	char *s_ptr = glob->stack->arr[idx];

	if (!ptr) {
		fprintf(stderr, "Could not fetch ptr to reg [%s]\n", reg);
		return 0;
	}

	memcpy(ptr, s_ptr, REG_BUF);
	return 1;
}
