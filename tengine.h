/**
 * @file: tengine.h
 * @desc: Declares functions that help in binding calls to string and
 *        calling the calls by name.
 */ 

#ifndef _ASE_T_ENG_
#define _ASE_T_ENG_

#define RESV    "resv"
#define BUFSIZE 128

#include "glob.h"
#include "parse.h"

typedef struct entry {
	char *f_id;
	int n_ops;
	int (*f_ptr)(glob_t *glob, char *buf, unsigned long size);
	struct entry *next;
} entry_t;

typedef struct table {
	entry_t *head;
	unsigned int entries;
} table_t;

int      call_by_name   (table_t *table, glob_t *glob,
                         char *buf, unsigned long size);
void     destroy_table  (table_t *table);
int      entry_exists   (table_t *table, char *f_id);
table_t *init_table     (void);
int      register_entry (table_t *table,
                         char *f_id,
                         int (*f_ptr)(glob_t *glob, char *buf, unsigned long size), int n_ops);

#endif
