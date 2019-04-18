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
void display(glob_t *glob, int flags, int mem, int reg, int stack) {
	if (!glob) {
		fprintf(stderr, "display(): glob - nullptr.\n");
		return;
	}

	if (flags && glob->flags) {
		printf("Flags:\n");
		printf("[CF]:[%d]\n",   glob->flags->cf);
		printf("[DF]:[%d]\n",   glob->flags->df);
		printf("[IF]:[%d]\n",   glob->flags->iif);
		printf("[PF]:[%d]\n",   glob->flags->pf);
		printf("[ZF]:[%d]\n\n", glob->flags->zf);
	}

	if (reg && glob->registers) {
		printf("Register:\n");
		printf("[AX]:[%s]\n",   glob->registers->ax);
		printf("[BX]:[%s]\n",   glob->registers->bx);
		printf("[CX]:[%s]\n",   glob->registers->cx);
		printf("[DX]:[%s]\n\n", glob->registers->dx);
	}

	if (stack && glob->stack) {
		if (glob->stack->top == -1) {
			goto skip;	
		}

		char **ptr = &glob->stack->arr[0];
		if (*ptr) {
			printf("Stack:\n");
		}

		while (*ptr) {
			char **ref = ptr;
			printf("[%p]:[%s]\n", (void*)&*ref, *ptr++);
			free(*ref);

			if (!*ptr) {
				printf("\n");
			}
		}
	}

	skip:
	if (mem && glob->mem) {
		if (!glob->mem->head) {
			return;
		}
		
		printf("Memory:\n");
		mem_nodes_t *node = glob->mem->head;
		
		while (node) {
			printf("[%d:%d] - [%s]\n", node->seg, node->offset, node->val);
			node = node->next;
		}
	}
}