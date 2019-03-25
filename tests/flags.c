/* Unit test for instructions that modify FLAG values. */

#include <assert.h>
#include <stdio.h>

#include "../flags.h"
#include "../glob.h"
#include "../mem.h"
#include "../parse.h"

int main(void) {
	FILE *fd = fopen("tests/ph", "r");
	if (!fd) {
		fprintf(stderr, "TEST: FLAGS - Could not open PH.\n");
		return 1;
	}

	glob_t *glob = init_glob(fd);
	if (!glob) {
		fprintf(stderr, "TEST: FLAGS - Glob is NULL.\n");
		return 1;
	}

	char l_1[] = "STC";
	char l_2[] = "STD";
	char l_3[] = "STI";
	char l_4[] = "CMC";
	char l_5[] = "CLC";
	char l_6[] = "CLD";
	char l_7[] = "CLI";

	parse_line(glob, l_1);
	set_flag(glob, NULL, 0);
	if (glob->flags->cf != 1) {
		fprintf(stderr, "TEST: FLAGS - Could not set CF value.\n");
		return 1;
	}

	parse_line(glob, l_2);
	set_flag(glob, NULL, 0);
	if (glob->flags->df != 1) {
		fprintf(stderr, "TEST: FLAGS - Could not set DF value.\n");
		return 1;
	}

	parse_line(glob, l_3);
	set_flag(glob, NULL, 0);
	if (glob->flags->iif != 1) {
		fprintf(stderr, "TEST: FLAGS - Could not set IF value.\n");
		return 1;
	}

	parse_line(glob, l_4);
	set_flag(glob, NULL, 0);
	if (glob->flags->cf != 0) {
		fprintf(stderr, "TEST: FLAGS - Could not run CMC:\n");
		return 1;
	}

	parse_line(glob, l_5);
	set_flag(glob, NULL, 0);
	if (glob->flags->cf != 0) {
		fprintf(stderr, "TEST: FLAGS - Could not clear CF flag.\n");
		return 1;
	}

	parse_line(glob, l_6);
	set_flag(glob, NULL, 0);
	if (glob->flags->df != 0) {
		fprintf(stderr, "TEST: FLAGS - Could not clear DF flag.\n");
		return 1;
	}

	parse_line(glob, l_7);
	set_flag(glob, NULL, 0);
	if (glob->flags->iif != 0) {
		fprintf(stderr, "TEST: FLAGS - Could not clear IF flag.\n");
		return 1;
	}
	

	fclose(fd);
	return 0;
}