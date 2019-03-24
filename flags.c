/**
 * @file: flags.c
 * @desc: Defines the following 8086 instructions:
 *        a) CLC
 *        b) CLD
 *        c) CLI
 *        d) CMC
 *        d) STC
 *        e) STD
 *        f) STI
 */ 

#include <stdio.h>
#include <string.h>

#include "flags.h"

/**
 * @desc  : Implements the CMC instruction.
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: int  - 0 if fail, 1 if success.
 */
int cmc(glob_t *glob, char *buf, unsigned long size) {
	if (!glob) {
		fprintf(stderr, "Invalid glob - points to NULL.\n");
		return 0;
	}

	glob->flags->cf = glob->flags->cf == 1 ? 0 : 1;
	return 1;
}

/**
 * @desc  : Clears the specified flag (unsets the bit).
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: int  - 0 if fail, 1 if success.
 */
int clear_flag(glob_t *glob, char *buf, unsigned long size) {
	if (!glob) {
		fprintf(stderr, "Invalid glob - points to NULL.\n");
		return 0;
	}

	const char *instr = glob->tokens[0];
	const char back   = instr[strlen(instr) - 1];

	switch (back) {
	case 'C': glob->flags->cf  = 0; return 1;      /* CLC */
	case 'D': glob->flags->df  = 0; return 1;      /* CLD */
	case 'I': glob->flags->iif = 0; return 1;      /* CLI */
	}

	return 0;
}

/**
 * @desc  : Returns the specified flag value.
 * @param : glob -
 *          flag - flag's value that is required.
 * @return: int  - -1 if fail, else the flag's value.
 */
int get_flag_val(glob_t *glob, char *flag) {
	if (!glob) {
		fprintf(stderr, "Invalid glob - points to NULL.\n");
		return -1;
	}

	if (!flag) {
		fprintf(stderr, "Invalid buf - points to NULL.\n");
		return -1;
	}

	switch (*flag) {
	case 'U': return glob->flags->af;
	case 'E': return glob->flags->cf;
	case 'I': return glob->flags->iif;
	case 'P': return glob->flags->pf;
	case 'Z': return glob->flags->zf;

	default: return -1;
	}

	return -1;
}

/**
 * @desc  : Sets the specified flag (sets the bit).
 * @param : glob -
 *          buf  - unused
 *          size - unused
 * @return: int  - 0 if fail, 1 if success.
 */
int set_flag(glob_t *glob, char *buf, unsigned long size) {
	if (!glob) {
		fprintf(stderr, "Invalid glob - points to NULL.\n");
		return 0;
	}

	const char *instr = glob->tokens[0];
	const char back   = instr[strlen(instr) - 1];

	switch (back) {
	case 'C': glob->flags->cf  = 1; return 1;      /* STC */
	case 'D': glob->flags->df  = 1; return 1;      /* STD */
	case 'I': glob->flags->iif = 1; return 1;      /* STI */
	}

	return 0;
}