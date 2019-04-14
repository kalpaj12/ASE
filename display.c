/**
 * @file: display.c
 * @desc: Defines functions that put data from memory, registers and
 *        stack onto the display.
 */ 

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "display.h"

/**
 * @desc  : Display data from memory, registers and stack.
 * @param : glob -
 * @return: void
 */ 
void display(glob_t *glob) {
	assert(glob);

	if (glob->flags) {
		printf("Flags:\n");
		printf("[CF]:[%d]\n",   glob->flags->cf);
		printf("[DF]:[%d]\n",   glob->flags->df);
		printf("[IF]:[%d]\n",   glob->flags->iif);
		printf("[PF]:[%d]\n",   glob->flags->pf);
		printf("[ZF]:[%d]\n\n", glob->flags->zf);
	}

	if (glob->registers) {
		printf("Register:\n");
		printf("[AX]:[%s]\n",   glob->registers->ax);
		printf("[BX]:[%s]\n",   glob->registers->bx);
		printf("[CX]:[%s]\n",   glob->registers->cx);
		printf("[DX]:[%s]\n\n", glob->registers->dx);
	}

	if (glob->stack) {
		char **ptr = &glob->stack->arr[0];
		if (ptr) {
			printf("Stack:\n");
		}

		while (*ptr) {
			char **t = ptr;
			printf("[%p]:[%s]\n", (void*)&*t, *ptr++);
			free(*t);
		}

		printf("\n");
	}

	if (glob->mem) {
		printf("Memory:\n");
		mem_nodes_t *node = glob->mem->head;
		
		while (node) {
			printf("[%d:%d] - [%s]\n", node->seg, node->offset, node->val);
			node = node->next;
		}
	}
}