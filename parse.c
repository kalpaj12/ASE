/**
 * @file: parse.c
 * @desc: Defines functions that help parse source lines and defines
 *        the following 8086 instructions:
 *        a) JC
 *        b) JE
 *        c) JP
 *        d) JNC
 *        e) JNE
 *        f) JNP
 *        g) JMP
 *        h) JCXZ
 */ 

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

/**
 * @desc  : Fetches the destination location from the source line.
 * @param : line - source line.
 *          buf  - buffer that receives this location.
 *          size - size of the buffer defined by the user.
 * @return: int  - 0 if fail, 1 if success.
 */
int fetch_dest(char *line, char *buf, unsigned long size) {
	memset(buf, 0, size);

	char *beg = strchr(line, ' ');
	char *end = strchr(line, ',');

	if (beg++ && end) {
		assert((end - beg) < size);
		memcpy(buf, beg, (end - beg));
		
		return 1;
	}

	return 0;
}

/**
 * @desc  : Fetches the instruction from the source line.
 * @param : line - source line.
 *          buf  - buffer that receives this instruction.
 *          size - size of the buffer defined by the user.
 * @return: int  - 0 if fail, 1 if success.
 */
int fetch_instr(char *line, char *buf, unsigned long size) {
	memset(buf, 0, size);
	char *ptr = line;

	if (ptr) {
		while (*ptr && size && *ptr != ' ') {
			ptr++;
		}

		assert((ptr - line) < size);
		memcpy(buf, line, (ptr - line));
		return 1;
	}

	fprintf(stderr, "Delim - null.\n");
	return 0;
}

/**
 * @desc  : Fetches the source location from the source line.
 * @param : line - source line.
 *          buf  - buffer that receives this location.
 *          size - size of the buffer defined by the user.
 * @return: int  - 0 if fail, 1 if success.
 */
int fetch_src(char *line, char *buf, unsigned long size) {
	memset(buf, 0, size);
	char *beg = strrchr(line, ' ');

	if (beg++) {
		char *end = beg;
		while (*end++);
		
		assert((end - beg) < size);
		memcpy(buf, beg, (end - beg));

		return 1;
	}

	return 0;
}

/**
 * @desc  : Returns the size of the register.
 * @param : reg - Size of the register that is required.
 * @return: int - 0 if fail, else the size - 8, 16.
 */
int get_reg_size(char *reg) {
	if (!reg) {
		fprintf(stderr, "Invalid reg - points to NULL.\n");
		return 0;
	}

	if (!is_loc_reg(reg)) {
		fprintf(stderr, "Specified reg [%s] is not a valid reg.\n", reg);
		return 0;
	}

	switch (reg[1]) {
	case 'H':               /* Upper 8 bit register. */
	case 'L': return 8;     /* Lower 8 bit register. */
	case 'X': return 16;    /* 16 bit register.      */
	}

	return 0;
}

/**
 * @desc  : Returns if the instruction is valid (supported).
 * @param : instr - instruction to validate.
 * @return: int   - 0 if no, 1 if yes.
 */ 
int is_instr_valid(char *instr) {
	if (!instr) {
		fprintf(stderr, "Invalid instr - points to NULL.\n");
	}

	int idx = 0;
	static char *v_instr[] = {"ADD", "MOV", "POP", "PUSH", "SUB", NULL};
	
	while (v_instr[idx]) {
		if (!strcmp(instr, v_instr[idx++])) {
			return 1;
		}
	}

	return 0;
}

/**
 * @desc  : Returns if the specified location is a register location.
 * @param : loc - Location to verify
 * @return: int - 0 for no, 1 for yes.
 */
int is_loc_reg(char *loc) {
	if (!loc) {
		fprintf(stderr, "Invalid loc - points to NULL.\n");
		return 0;
	}

	if (strlen(loc) != 2) {
		return 0;
	}

	if (loc[0] >= 'A' && loc[0] <= 'D') {
		return (loc[1] == 'H' || loc[1] == 'L' || loc[1] == 'X');
	}

	return 0;
}

/**
 * @desc  : Returns if the specified location is a mem location.
 * @param : loc - location to check for.
 * @return: int - 0 if no, 1 if yes.
 */
int is_loc_addr(char *loc) {
	if (!loc) {
		fprintf(stderr, "Invalid loc - points to NULL.\n");
		return 0;
	}

	int ret = (loc[0] == '[' && loc[strlen(loc) - 1] == ']') ?
	          1 : 0;

	for (int i = 1; i <= strlen(loc) - 2; i++) {
		if (!isalnum(loc[i])) {
			ret = 0;
		}
	}

	return ret;
}

/**
 * @desc  : Validates if given string is a valid hex.
 * @param : hex - Hex to validate
 * @return: int - 0 if no, 1 if yes.
 */
int is_valid_hex(char *hex) {
	char *ptr = hex;
	while (*ptr) {
		if ((!isalpha(*ptr) && !isdigit(*ptr)) || (isalpha(*ptr) && toupper(*ptr) > 'F')) {
			return 0;
		}
		
		ptr++;
	}

	return 1;
}

/**
 * @desc  : Returns if the specified operand is valid.
 * @param : op  - operand to validate.
 * @return: int - 0 if no, 1 if yes.
 */
int is_valid_op(char *op) {
	if (!op) {
		fprintf(stderr, "Invalid op - points to NULL.\n");
		return 0;
	}

	/**
	 * An operand is valid if it is:
	 * 	a) A value (hex?)
	 * 	b) Is a memory location
	 * 	c) Is a register
	 */
	return is_valid_hex(op) ^ is_loc_addr(op) ^ is_loc_reg(op);
}

/**
 * @desc  : Implements the JMP instruction.
 * @param : glob -
 *          buf  - unused for JMP.
 *                 set to REG_CX for JCXZ.
 *                 or the resp. value for JC/JE/JP.
 *          size - set to -1 for JNC/JNE/JPE.
 * @return: 0 if fail, 1 if success.
 */
int jump(glob_t *glob, char *buf, unsigned long size) {
	
	if (buf) {
		switch (*buf) {
		/* buf - REG_CX - JCXZ */
		case 'C': {
			int diff = strcmp(buf, REG_CX);
			if (!diff) {
				char val[1024];
				if (!get_reg_val(glob, REG_CX, val, sizeof(val))) {
					fprintf(stderr, "Could not fetch CX contents.\n");
					return 0;
				}

				if (strcmp(val, "0") != 0) {
					/* JCXZ condition failed. */
					return 1;
				} else {
					/* Allow jump execution. */
					break;
				}
			}
		}

		/* Carry flag - FLAG_CF - "EF" */
		case 'E': {
			/* JNC */
			if (size == -1 && glob->flags->cf == 0) {
				break;
			/* JC */
			} else if (size != -1 && glob->flags->cf == 1) {
				break;
			} else {
				return 1;
			}
		}

		/* Parity flag - FLAG_PF - "PF" */
		case 'P': {
			/* JPE */
			if (size == -1 && glob->flags->pf == 0) {
				break;
			/* JP */
			} else if (size != -1 && glob->flags->pf == 1) {
				break;
			} else {
				return 1;
			}
		}

		/* Zero flag - FLAG_ZF - "ZF" */
		case 'Z': {
			/* JNE */
			if (size == -1 && glob->flags->zf == 0) {
				break;
			/* JE */
			} else if (size != -1 && glob->flags->zf == 1) {
				break;
			} else {
				return 1;
			}
		}
		}
	}

	/* Jump */
	char line[1024], to_label[1024];
	memcpy(to_label, glob->tokens[1], sizeof(to_label));

	while (fgets(line, sizeof(line), glob->fd) != NULL) {
		if (should_skip_ln(line)) {
			continue;
		}

		/* Lets hope that the parser is good enough. */
		if (!parse_line(glob, line)) {
			fprintf(stderr, "Could not parse line.\n");
			return 0;
		}

		int diff = strcmp(glob->label, to_label);
		if (diff == 0) {
			fseek(glob->fd, -(glob->c_len), SEEK_CUR);
			return 1;
		}
	}

	return 0;
}

/**
 * @desc  : Implements the JCXZ instruction.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: 0 if fail, 1 if success.
 */
int jump_cx(glob_t *glob, char *buf, unsigned long size) {
	return jump(glob, REG_CX, size);
}

/**
 * @desc  : Implements the JC/JE/JP instruction.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: 0 if fail, 1 if success.
 */
int jump_jx(glob_t *glob, char *buf, unsigned long size) {
	const char *instr = glob->tokens[0];
	const char back   = instr[strlen(instr) - 1];

	switch (back) {
	case 'C': return jump(glob, FLAG_CF, size);                /* JC */
	case 'E': return jump(glob, FLAG_ZF, size);                /* JE */
	case 'P': return jump(glob, FLAG_PF, size);                /* JP */
	}

	return 0;
}

/**
 * @desc  : Implements the JNC/JNE/JPE instruction.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: 0 if fail, 1 if success.
 */
int jump_jnx(glob_t *glob, char *buf, unsigned long size) {
	const char *instr = glob->tokens[0];
	const char back   = instr[strlen(instr) - 1];

	switch (back) {
	case 'C': return jump(glob, FLAG_CF, (unsigned long)-1);   /* JNC */
	case 'E': return jump(glob, FLAG_ZF, (unsigned long)-1);   /* JNE */
	case 'P': return jump(glob, FLAG_PF, (unsigned long)-1);   /* JPE */
	}

	return 0;
}

/**
 * @desc  : Tokenises the given source line into [instr] [op1] [op2].
 * @param : glob -
 *          line - source line.
 * @return: int - 0 if fail, 1 if success.
 */
int parse_line(glob_t *glob, char *line) {
	assert(glob && line);
	for (int i = 0; i < 3; i++) {
		assert(glob->tokens[i]);
	}

	memset(glob->label, 0, 128);

	glob->c_line++;
	glob->p_len = glob->c_len;
	glob->c_len = strlen(line);

	int i = 0;
	int flag = 0;
	char *ptr = strtok(line, " ");

	while (ptr) {
		/* Skip if comment line begins. */
		if (*ptr == ';') {
			break;
		}

		/* There's a space between label and colon? */
		if (*ptr == ':') {
			fprintf(stderr, "Valid label syntax: Label: [instr] [operands].\n");
			return 0;
		}

		/* There's no instruction that takes more than 2 operands. */
		if (i > 2) {
			fprintf(stderr, "More than required tokens.\n");
			return 0;
		}

		if (i == 0) {
			char *x = ptr;
			while (*x) {
				*x = toupper(*x);
				x++;
			}
		}

		if (*ptr == ',') {
			/* Are we expecting a comma? Are we parsing op2? */
			if (flag && i > 1) {
				flag = 0;
				goto l1;
			} else {
				fprintf(stderr, "Unexpected character [,].\n");
				return 0;
			}
		}

		/* Token can begin only with alpha-numeric chars or a '['. */
		if (!isalnum(*ptr) && *ptr != '[') {
			fprintf(stderr, "Unexpected character [%c].\n", *ptr);
			return 0;
		}

		/**
		 * Check if comment line begins. No space between op and ';' perhaps?
		 * Eg: instr op1 op2; This is a comment line.
		 */
		char *end = strchr(ptr, ';');
		if (end) {
			/* Clip off the end and return */
			*end = '\0';
			memcpy(glob->tokens[i], ptr, 128);
			return 1;
		}

		char *back = &ptr[strlen(ptr) - 1];
		/* Check for label. */
		if (*back == ':') {
			*back = '\0';
			memcpy(glob->label, ptr, 128);
			/* Do not count label as a token. */
			goto l1;
		}

		if (!isalnum(*back)) {
			*back = '\0';
		} else {
			/* We're expecting a comma. */
			flag = 1;
		}

		memcpy(glob->tokens[i], ptr, 128);
		i++;

		l1:
		ptr = strtok(NULL, " ");
	}

	return 1;
}

/**
 * @desc  : Determines if the line should be skipped.
 * @param : line - line to verify.
 * @return: int  - 0 if no, 1 if yes.
 * 
 * 
 */
int should_skip_ln(char *line) {
	return (!line || line[0] == ';' || line[0] == '\n');
}

/**
 * @desc  : Used by jump instructions to step back.
 * @param : glob -
 * @return: int  - 0 if fail, 1 if success.
 */
int step_back(glob_t *glob) {
	if (!glob) {
		fprintf(stderr, "Invalid glob - points to NULL.\n");
		return 0;
	}

	if (!glob->fd) {
		fprintf(stderr, "Invalid fd - points to NULL.\n");
		return 0;
	}

	return fseek(glob->fd, -(glob->c_len + glob->p_len), SEEK_CUR) == 0;
}