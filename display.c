/**
 * @file: display.c
 * @desc: Defines functions that put data from memory, registers and
 *        stack onto the display.
 */ 

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "display.h"

/**
 * @desc  : Display data from memory, registers and stack.
 * @param : glob -
 * @return: void
 */ 
void display(glob_t *glob) {
	assert(glob);

	if (glob->flags) {
		printf("Flags :\n");
		printf("[Flag]:[Bit value] - [CF]:[%d]\n", glob->flags->cf);
		printf("[Flag]:[Bit value] - [DF]:[%d]\n", glob->flags->df);
		printf("[Flag]:[Bit value] - [IF]:[%d]\n", glob->flags->iif);
		printf("[Flag]:[Bit value] - [PF]:[%d]\n\n", glob->flags->pf);
	}

	if (glob->registers) {
		printf("Register:\n");
		printf("[%s] - [%s]\n", REG_AX, glob->registers->ax);
		printf("[%s] - [%s]\n", REG_BX, glob->registers->bx);
		printf("[%s] - [%s]\n", REG_CX, glob->registers->cx);
		printf("[%s] - [%s]\n", REG_DX, glob->registers->dx);
	}

	if (glob->stack) {
		int idx = glob->stack->top;
		idx > -1 ? printf("\nStack :\n") : idx;

		for (int i = 0; i <= idx; i++) {
			printf("[Addr]:[Value] - [%p]:[%s]\n", (void *)&glob->stack->arr[i],
			                            (char *)glob->stack->arr[i]);
			free(glob->stack->arr[i]);
			i == idx ? printf("\nNote: Stack memory cleared.\n") : 0;
		}
	}
}