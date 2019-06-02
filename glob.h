/**
 * @file: glob.h
 * @desc: Declares structures and functions that hold register and
 *        stack data.
 */

#ifndef _ASE_GLOB_H_
#define _ASE_GLOB_H_

#include <stdio.h>

#define BUF_SZ 128

#define TERM_RED   "\x1b[31m"
#define TERM_GREEN "\x1b[32m"
#define TERM_RESET "\x1b[0m"

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
	/**
	 * Indicates if a flag has changed.
	 * if f_ch[0] is set, it means af (aux flag) has changed.
	 * if f_ch[1] is set, it means cf (carry flag) has changed
	 * and so on.
	 */
	int f_ch[8];
	int af, cf, df, iif, of, pf, sf, zf;
} flags_t;

typedef struct mem_nodes {
	char val[BUF_SZ];
	int seg, offset, addr;
	struct mem_nodes *next;
} mem_nodes_t;

typedef struct mem {
	int si, di;
	int ds, es, warned;
	mem_nodes_t *head;
} mem_t;

typedef struct stack {
	int top;
	char *arr[BUF_SZ];
} stack_t;

typedef struct registers {
	char ax[BUF_SZ], bx[BUF_SZ],
         cx[BUF_SZ], dx[BUF_SZ];
} registers_t;

typedef struct glob {
	int idx;
	struct label_loc {
		int line;
		char label[BUF_SZ];
	} label_locs[256];

	int debug;
	int bpnt, n_op;
	FILE *fd;

	/**
	 * label    - Did user specify any label for the present line?
	 * to_label - Label to jump - filled when from_reg jump instr is specified.
	 * tokens   - [instr] [op1] [op2]
	 */
	char tokens[3][BUF_SZ];
	char label[BUF_SZ], to_label[BUF_SZ];

	/**
	 * c_line - Current source line number.
	 * c_len  - Current source line length.
	 * p_len  - Previous source line length.
	 *
	 * Previous line offset = -1 * (c_len + p_len)
	 */
	int c_line, c_len, p_len;

	mem_t *mem;
	flags_t *flags;
	stack_t *stack;
	registers_t *registers;
} glob_t;

mem_nodes_t *add_to_mem   (glob_t *glob, int seg, int offset);
void         destroy_glob (glob_t *glob);
mem_nodes_t *get_mem_node (glob_t *glob, int addr);
int          get_op_val   (glob_t *glob, char *op, char *buf, unsigned long size);
char        *get_reg_ptr  (glob_t *glob, char *reg);
glob_t      *init_glob    (FILE   *fd);
int          lahf         (glob_t *glob, char *buf, unsigned long size);
int          org          (glob_t *glob, char *buf, unsigned long size);
int          sahf         (glob_t *glob, char *buf, unsigned long size);

#endif