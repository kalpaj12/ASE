/* Unit test for instructions that move data [MOV]. */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../glob.h"
#include "../mem.h"
#include "../parse.h"

int main(void) {
	FILE *fd = fopen("tests/ph", "r");
	if (!fd) {
		fprintf(stderr, "TEST: MOV - Could not open PH.\n");
		return 1;
	}

	glob_t *glob = init_glob(fd);
	if (!glob) {
		fprintf(stderr, "TEST: MOV - Glob is NULL.\n");
		return 1;
	}

	char l_1[] = "MOV AX, 1234H";
	char l_2[] = "MOV BX, AX";
	char l_3[] = "MOV CX, DX";

	parse_line(glob, l_1);
	move(glob, NULL, 0);
	if (strcmp(glob->registers->ax, "1234") != 0) {
		fprintf(stderr, "Test: MOV - Failed to set AX value.\n");
		return 1;
	}

	parse_line(glob, l_2);
	move(glob, NULL, 0);
	if (strcmp(glob->registers->bx, "1234") != 0) {
		fprintf(stderr, "Test MOV: Failed to set BX value.\n");
		return 1;
	}

	parse_line(glob, l_3);
	move(glob, NULL, 0);
	if (strcmp(glob->registers->cx, "") != 0) {
		fprintf(stderr, "TEST MOV: Failed to set DX value.\n");
		return 1;
	}

	fclose(fd);
	return 0;
}