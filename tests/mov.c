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
	char l_4[] = "MOV DL, 34H";
	char l_5[] = "MOV [1234H], 34H";
	char l_6[] = "MOV [1245], 34H";
	char l_7[] = "MOV [12], 34H";

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

	parse_line(glob, l_4);
	move(glob, NULL, 0);
	if (strcmp(glob->registers->dx, "34") != 0) {
		fprintf(stderr, "TEST MOV: Failed to set DL value.\n");
		return 1;
	}

	int i = 0;
	int arr[] = {12, 1234, 1245};

	parse_line(glob, l_5);
	move(glob, NULL, 0);

	parse_line(glob, l_6);
	move(glob, NULL, 0);

	parse_line(glob, l_7);
	move(glob, NULL, 0);

	mem_nodes_t *node = glob->mem->head;
	while (node) {
		assert(node->seg == 0);
		assert(node->offset == arr[i]);
		node = node->next;
		i++;
	}

	assert(i);
	fclose(fd);
	return 0;
}