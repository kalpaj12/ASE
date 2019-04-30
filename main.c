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
	int flags = 0, mem = 0, reg = 0, stack = 0;
	while ((opt = getopt(argc, argv, "afmrs")) != -1) {
		switch (opt) {
		case 'a': {
			flags = mem = reg = stack = 1;
			break;
		}

		case 'f': flags = 1; break;
		case 'm': mem   = 1; break;
		case 'r': reg   = 1; break;
		case 's': stack = 1; break;
		}
	}

	(void) display(glob, flags, mem, reg, stack);
	for (; optind < argc; optind++) {
		if (!strstr(argv[optind], ".asm")) {
			fprintf(stderr, "Ignore extra argument: %s\n", argv[optind]);
		}
	}
}

void show_flags() {
	fprintf(stderr, "Supported flags: \n\
		-a : Enable all (below) flags \n\
		-f : Show flag contents \n\
		-m : Show memory contents \n\
		-r : Show register contents \n\
		-s : Show stack contents\n");
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Error: Please specify minimum number [1] of args.\n");
		fprintf(stderr, "ASE [Source File] [Arguments]\n\n");
		show_flags();
		return 1;
	}

	if (argc == 2) {
		fprintf(stderr, "Warning: Missing program flag(s)?\n");
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

		int ret = call_by_name(table, glob, NULL, (unsigned long)512);
		if (ret == -1) {
			break;
		}

		if (!ret) {
			flag = 1;
			break;
		}
	}

	if (flag) {
		fprintf(stderr, "Emulator halted due to an error in line %d. State preserved.\n\n",
			++glob->c_line);
	}

	/* Program ends here. */
	parse_args(glob, argc, argv);

	/* Close existing file handles and clear out alloc'ed memory. */
	fclose(fd);
	destroy_glob(glob);
	destroy_table(table);

	return flag;
}
