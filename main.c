/**
 * @file: main.c
 * @desc: Front-end for ASE.
 * 
 * @author: Pawan Kartik
 *          Manipal University - 2019
 */

#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "bind.h"
#include "display.h"
#include "glob.h"
#include "mem.h"
#include "parse.h"
#include "stack.h"
#include "tengine.h"

void parse_args(glob_t *glob, int argc, char **argv) {
	int opt;
	while ((opt = getopt(argc, argv, "d")) != -1) {
		switch (opt) {
		case 'd': display(glob); break;
		}
	}

	for (; optind < argc; optind++) {
		if (!strstr(argv[optind], ".asm")) {
			fprintf(stderr, "Ignore extra argument: %s\n", argv[optind]);
		}
	}
}

void show_flags() {
	fprintf(stderr, "Supported flags: \n\
		-d : Display register and stack contents. \n\
		-c : Flush memory out.\n");
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Error: Please specify minimum number [1] of args.\n");
		fprintf(stderr, "ASE [Source File] [Arguments]\n\n");
		show_flags();
		return 1;
	}

	table_t *table = init_table();
	bind_calls(table);
	FILE *fd = fopen(argv[1], "r");
	
	if (!fd) {
		fprintf(stderr, "Could not open specified file.\n");
		return 1;
	}

	int flag = 0;
	char line[1024];
	glob_t *glob = init_glob(fd);
	memset(line, 0, sizeof(line));

	while (fgets(line, sizeof(line), fd) != NULL) {
		if (should_skip_ln(line)) {
			continue;
		}
		
		if (!parse_line(glob, line)) {
			fprintf(stderr, "Could not parse line.\n");
			flag = 1;
			break;
		}

		if (!call_by_name(table, glob, NULL, (unsigned long)512)) {
			flag = 1;
			break;
		}
	}

	if (flag) {
		fprintf(stderr, "Emulator halted due to an error. State preserved.\n");
	}

	/* Program ends here. */
	parse_args(glob, argc, argv);

	/* Close existing file handles and clear out alloc'ed memory. */
	fclose(fd);
	destroy_glob(glob);
	destroy_table(table);

	return flag;
}
