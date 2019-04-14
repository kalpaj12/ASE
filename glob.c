/**
 * @file: glob.c
 * @desc: Declares structures and functions that hold register and
 *        stack data.
 */

#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "glob.h"
#include "parse.h"

/**
 * @desc  : Allows user to move data to specified mem addr.
 * @param : glob   -
 *          seg    - segment address.
 *          offset - offset address.
 *          PA     - (seg * 10) + offset
 *          val    - value to copy.
 * @return: int    - 0 if fail, 1 if success.
 */
int add_to_mem(glob_t *glob, int seg, int offset, char *val, int conv_req) {
	if (!glob) {
		fprintf(stderr, "add_to_mem(): glob - null ptr.\n");
		return 0;
	}

	mem_nodes_t *node = malloc(sizeof(mem_nodes_t));
	if (!node) {
		fprintf(stderr, "add_to_mem(): malloc failure.\n");
		return 0;
	}

	node->next = NULL;
	node->seg  = seg;
	node->offset = offset;

	/* 8086's PA is of 20 bits */
	int base = conv_req ? 0 : 16;
	node->addr = (seg * 10) + offset;
	assert(strlen(val) <= 4);
	(void) sprintf(node->val, "%x", (unsigned int)strtol(val, NULL, base));

	if (!glob->mem->head) {
		glob->mem->head = node;
		return 1;
	}

	mem_nodes_t *curr = glob->mem->head, *prev = NULL;
	while (curr) {
		if (node->addr >= curr->addr) {
			prev = curr;
			curr = curr->next;
		}

		node->next = curr;
		/* Appending to the beginning of the list? */
		if (!prev) {
			glob->mem->head = node;
			return 1;
		}

		prev->next = node;
		return 1;
	}

	return 0;
}

/**
 * @desc  : Destory parent structure.
 * @param : glob -
 * @return: void
 */ 
void destroy_glob(glob_t *glob) {
	if (!glob) {
		fprintf(stderr, "Invalid glob - points to NULL.\n");
		return;
	}

	fclose(glob->fd);

	mem_nodes_t *node = glob->mem->head;
	while (node) {
		mem_nodes_t *temp = node;
		node = node->next;
		free(temp);
	}

	free(glob->mem);
	free(glob->flags);
	free(glob->stack);
	free(glob->registers);
	free(glob);
}

/**
 * @desc  : Returns the pointer to the specified register.
 * @param : glob -
 *          reg  - register's pointer that is required.
 * @return: char*
 */ 
char *get_reg_ptr(glob_t *glob, char *reg) {
	if (glob && reg) {
		char *ptr  = NULL;

		/* Convert 8 bit register to 16 bit. */
		char *back = &reg[strlen(reg) - 1];
		if (*back == 'H' || *back == 'L') {
			*back = 'X';
		} else if (*back != 'X') {
			/* back - neither H, L nor X. */
			return NULL;
		}

		int diff = abs(strcmp(reg, REG_AX));

		switch (diff) {
		case 0: ptr = glob->registers->ax; break;
		case 1: ptr = glob->registers->bx; break;
		case 2: ptr = glob->registers->cx; break;
		case 3: ptr = glob->registers->dx; break;

		default: return NULL;
		}

		if (!ptr) {
			ptr = malloc(REG_BUF);
		}

		return ptr;
	}

	return NULL;
}

/**
 * @desc  : Copies the specified register's value to buf.
 * @param : glob -
 *          reg  - register's value that is required.
 *          buf  - buffer receiving the value.
 *          size - size of the buffer.
 * @return: 0 if fail, 1 if success.
 */ 
int get_reg_val(glob_t *glob, char *reg, char *buf, unsigned long size) {
	if (glob && reg && buf) {
		char *ptr = get_reg_ptr(glob, reg);
		if (!ptr) {
			return 0;
		}

		memcpy(buf, ptr, size);
		return 1;
	}

	return 0;
}

/**
 * @desc  : Init parent and child structures.
 * @param : none
 * @return: void
 */
glob_t *init_glob(FILE *fd) {
	if (!fd) {
		fprintf(stderr, "Invalid fd - points to null.\n");
		return NULL;
	}

	glob_t *glob = malloc(sizeof(glob_t));
	if (!glob) {
		return NULL;
	}
	
	glob->mem = malloc(sizeof(mem_t));
	glob->flags = malloc(sizeof(flags_t));
	glob->stack = malloc(sizeof(stack_t));
	glob->registers = malloc(sizeof(registers_t));

	assert(glob->mem);
	assert(glob->flags);
	assert(glob->stack);
	assert(glob->registers);

	glob->fd = fd;
	glob->stack->top = -1;
	glob->c_line = glob->p_len = 0;

	memset(glob->flags, 0, sizeof(flags_t));
	return glob;
}

/**
 * @desc  : Returns the ul value of the specified register.
 * @param : glob -
 *          reg  - register's value that is required.
 *          buf  - buffer receiving the value.
 * @return: int  - 0 if fail, 1 if success.
 */
int reg_to_ul(glob_t *glob, char *reg, unsigned long *buf) {
	if (glob && reg) {
		char *ptr = get_reg_ptr(glob, reg);

		if (ptr) {
			*buf = strtol(ptr, NULL, 16);
			return 1;
		}
	}

	fprintf(stderr, "reg_to_ul(): Null ptrs received.\n");
	return 0;
}

/**
 * @desc  : Sets the specified register's value.
 * @param : glob -
 *          reg  - register's value that is to be set.
 *          val  - value to set.
 *          conv_req - should the value be converted to hex?
 * @return: 0 if fail, 1 if success.
 */
int set_reg_val(glob_t *glob, char *reg, char *val, int conv_req) {
	if (glob && reg && val) {
		char buf[128];
		char *ptr = get_reg_ptr(glob, reg);

		if (!ptr) {
			fprintf(stderr, "set_reg_val(): ptr to reg is null.\n");
			return 0;
		}

		if (conv_req) {
			sprintf(buf, "%x", (unsigned int)strtol(val, NULL, 0));
		}

		char *cp = conv_req ? buf : val;
		memcpy(ptr, cp, REG_BUF);
		return 1;
	}

	return 0;
}