/**
 * @file: tengine.c
 * @desc: Declares functions that help in binding calls to string and
 *        calling the calls by name.
 */ 

#include <ctype.h>
#include <malloc.h>
#include <string.h>

#include "tengine.h"

/**
 * @desc  : Call a function from table by it's name.
 * @param : table - table containing the entries.
 *          glob  -
 *          line  - line from source file.
 *          buf   - unused
 *          size  - unused
 * @return: int   - 0 if fail, 1 if success.
 */
int call_by_name(table_t *table, glob_t *glob, char *buf, unsigned long size) {
	int x = 0;
	const int klen = strlen(glob->tokens[0]);
	for (int i = 0; i < klen; i++) {
		if (isspace(glob->tokens[0][i])) {
			x++;
		}
	}

	if (x == klen) {
		return 1;
	}

	entry_t *entry = table->head;
	while (entry) {
		if (!strcmp(entry->f_id, glob->tokens[0])) {
			return entry->f_ptr(glob, buf, size);
		}

		entry = entry->next;
	}

	fprintf(stderr, "Invalid entry [%s]: reached end of the table.\n",
	        glob->tokens[0]);
	return 0;
}

/**
 * @desc  : Release memory allocated to the table and its entries.
 * @param : table - table containing the entries.
 * @return: void
 */
void destroy_table(table_t *table) {
	if (!table) {
		return;
	}

	entry_t *entry = table->head;
	while (entry) {
		free(entry->f_id);
		entry = entry->next;
		free(entry);
	}
}

/**
 * @desc  : Returns if the specified function's entry exists.
 * @param : table - table containing the entries.
 *          f_id  - binding function name.
 * @return: int   - 0 for fail, 1 for success.
 */
int entry_exists(table_t *table, char *f_id) {
	if (!table) {
		return 0;
	}

	entry_t *entry = table->head;
	while (entry) {
		if (!strcmp(entry->f_id, f_id)) {
			return 1;
		}

		entry = entry->next;
	}

	return 0;
}

/**
 * @desc  : Allocate memory to table and return a pointer.
 * @param : none
 * @return: table* - pointer to the table.
 */
table_t *init_table(void) {
	table_t *table = malloc(sizeof(table_t));
	return table;
}

/**
 * @desc  : Bind a function with specified name.
 * @param : table - table containing the entries.
 *          f_id  - binding function name.
 *          glob  -
 *          line  - line from source file.
 *          buf   - unused
 *          size  - unused
 * @return: int   - 0 if fail, 1 if success.
 */
int register_entry(table_t *table, char *f_id,
                    int (*f_ptr)(glob_t *glob, char *buf, unsigned long size)) {
	if (!table) {
		fprintf(stderr, "table - nullptr: register_entry");
		return 0;
	}

	entry_t *entry = malloc(sizeof(entry_t));
	if (entry) {
		entry->f_id = malloc(BUFSIZE);
		memcpy(entry->f_id, f_id, BUFSIZE);
		entry->f_ptr = f_ptr;
		table->entries++;

		entry->next = table->head;
		table->head = entry;

		return 1;
	} else {
		fprintf(stderr, "malloc() fail: register_entry");
	}

	return 0;
}
