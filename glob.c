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
 * @return: mem_nodes_t
 */
mem_nodes_t* add_to_mem(glob_t *glob, int seg, int offset) {
	if (!glob) {
		fprintf(stderr, "add_to_mem(): glob - null ptr.\n");
		return NULL;
	}

	mem_nodes_t *node = malloc(sizeof(mem_nodes_t));
	if (!node) {
		fprintf(stderr, "add_to_mem(): malloc failure.\n");
		return NULL;
	}

	node->next = NULL;
	node->seg  = seg;
	node->offset = offset;

	node->addr = (seg * 10) + offset;
	if (!glob->mem->head) {
		glob->mem->head = node;
		return node;
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
			return node;
		}

		prev->next = node;
		return node;
	}

	return NULL;
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
 * @desc  : Returns the memory value pointed by addr.
 * @param : glob -
 *          addr - address of the value.
 *          buf  - buffer that receives the data.
 *          size - size of the buffer.
 * @return: int  - 0 if fail, 1 if success.
 */ 
int get_mem_val(glob_t *glob, int addr, char *buf, unsigned long size) {
	if (!glob) {
		fprintf(stderr, "get_mem_val(): glob - nullptr.\n");
		return 0;
	}

	mem_nodes_t *node = glob->mem->head;
	while (node) {
		int pa = (node->seg * 10) + node->offset;
		if (pa == addr) {
			memcpy(buf, node->val, size);
			return 1;
		}

		node = node->next;
	}

	return 0;
}

/**
 * @desc  : Gets the specified operand's value.
 * @param : glob -
 *          op   - operand
 *          buf  - buffer that receives the value.
 *          size - size of the buffer.
 * @return: int  - 0 if fail, 1 if success.
 */ 
int get_op_val(glob_t *glob, char *op, char *buf, unsigned long size) {
	if (!glob) {
		fprintf(stderr, "get_op_val(): glob - nullptr.\n");
		return 0;
	}

	if (is_op_addr(op)) {
		char addr[BUF_SZ];
		memset(addr, 0, sizeof(addr));
		memcpy(addr, &op[1], strlen(op) - 2);

		return get_mem_val(glob, (int)strtol(addr, NULL, 0), buf, size);
	}

	if (is_op_reg(op)) {
		char *ptr = get_reg_ptr(glob, op);
		memcpy(buf, ptr, REG_BUF);
		return 1;
	}

	/* Operand is a literal */
	char *lchar = &op[strlen(op) - 1];
	if (*lchar == 'H' || *lchar == 'h') {
		*lchar = '\0';

		/* Validate hex */
		if (!is_valid_hex(op)) {
			fprintf(stderr, "get_op_val(): Invalid hex literal.\n");
			return 0;
		}

		memcpy(buf, op, size);
		return 1;
	} else {
		/* Need to convert the literal to hex */
		const int ksz = strlen(op);
		for (int i = 0; i < ksz; i++) {
			if (!isdigit(op[i])) {
				fprintf(stderr, "get_op_val(): Invalid str literal [%s].\n", op);
				return 0;
			}
		}

		sprintf(buf, "%x", (int)strtol(op, NULL, 0));
		return 1;
	}

	return 0;
}

/**
 * @desc  : Returns the pointer to the specified register.
 * @param : glob -
 *          reg  - register's pointer that is required.
 * @return: char*
 */ 
char *get_reg_ptr(glob_t *glob, char *reg) {
	if (!glob || !reg) {
		fprintf(stderr, "get_reg_ptr(): nullptr received.\n");
		return NULL;
	}

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