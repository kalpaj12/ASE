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
#include "parse.h"

/**
 * @desc  : Display specified register value.
 * @param : reg - Register's value needed to display.
 *          val - Value of register.
 * @return: void
 */
void display_reg_val(char *reg, char *val) {
	char reg_[1024] = {0};
	memcpy(reg_, reg, sizeof(reg_));

	char *back = &reg_[strlen(reg_) - 1];
	const int klen = strlen(val);
	*back = (klen <= 2 && klen >= 1) ? 'L' : 'X';
	printf("[%s] - [%s]\n", reg_, val);
}

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
		display_reg_val(REG_AX, glob->registers->ax);
		display_reg_val(REG_BX, glob->registers->bx);
		display_reg_val(REG_CX, glob->registers->cx);
		display_reg_val(REG_DX, glob->registers->dx);
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