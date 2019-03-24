/**
 * @file: glob.h
 * @desc: Declares structures and functions that hold register and
 *        stack data.
 */

#ifndef _ASE_GLOB_H_
#define _ASE_GLOB_H_

#include <stdio.h>

#define REG_BUF 32

#define FLAG_AF "UF"
#define FLAG_CF "EF"
#define FLAG_IF "IF"
#define FLAG_PF "PF"
#define FLAG_ZF "ZF"

#define REG_AX "AX"
#define REG_BX "BX"
#define REG_CX "CX"
#define REG_DX "DX"

typedef struct flags {
	int af, cf, df, iif, pf, zf;
} flags_t;

typedef struct stack {
	int top;
	char *arr[128];
} stack_t;

typedef struct registers {
	char ax[REG_BUF], bx[REG_BUF],
         cx[REG_BUF], dx[REG_BUF];
} registers_t;

typedef struct glob {
	FILE *fd;

	/**
	 * label    - Did user specify any label for the present line?
	 * to_label - Label to jump - filled when a jump instr is specified.
	 * tokens   - [instr] [op1] [op2]
	 */
	char tokens[3][128];
	char label[128], to_label[128];

	/**
	 * c_line - Current source line number.
	 * c_len  - Current source line length.
	 * p_len  - Previous source line length.
	 *
	 * Previous line offset = -1 * (c_len + p_len)
	 */
	int c_line, c_len, p_len;

	flags_t *flags;
	stack_t *stack;
	registers_t *registers;
} glob_t;

void    destroy_glob (glob_t *glob);
char   *get_reg_ptr  (glob_t *glob, char *reg);
int     get_reg_val  (glob_t *glob, char *reg, char *buf, unsigned long size);
glob_t *init_glob    (FILE   *fd);
int     set_reg_val  (glob_t *glob, char *reg, char *val);

#endif