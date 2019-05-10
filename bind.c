/**
 * @file: bind.c
 * @desc: Defines function that helps in binding instruction set to
 *        strings.
 */ 

#include <assert.h>

#include "bind.h"

/**
 * @desc  : Bind instruction set to strings.
 * @param : table - table containing the entries.
 * @return: void
 */ 
void bind_calls(table_t *table) {
	
	if (!table) {
		fprintf(stderr, "nullptr - table.\n");
		return;
	}

	assert(register_entry(table, "ADD",  math_op));
	assert(register_entry(table, "CLC",  clear_flag));
	assert(register_entry(table, "CLD",  clear_flag));
	assert(register_entry(table, "CLI",  clear_flag));
	assert(register_entry(table, "CMC",  cmc));
	assert(register_entry(table, "CMP",  math_op));
	assert(register_entry(table, "DEC",  unary));
	assert(register_entry(table, "HLT",  hlt));
	assert(register_entry(table, "IN",   nop));
	assert(register_entry(table, "INC",  unary));
	assert(register_entry(table, "JCXZ", jump_cx));
	assert(register_entry(table, "JC",   jump_jx));
	assert(register_entry(table, "JE",   jump_jx));
	assert(register_entry(table, "JNC",  jump_jnx));
	assert(register_entry(table, "JNE",  jump_jnx));
	assert(register_entry(table, "JP",   jump_jx));
	assert(register_entry(table, "JPE",  jump_jx));
	assert(register_entry(table, "JMP",  jump));
	assert(register_entry(table, "MOV",  move));
	assert(register_entry(table, "MUL",  math_op));
	assert(register_entry(table, "NEG",  neg));
	assert(register_entry(table, "NOP",  nop));
	assert(register_entry(table, "OUT",  nop));
	assert(register_entry(table, "POP",  pop));
	assert(register_entry(table, "PUSH", push));
	assert(register_entry(table, "STC",  set_flag));
	assert(register_entry(table, "STD",  set_flag));
	assert(register_entry(table, "STI",  set_flag));
	assert(register_entry(table, "SUB",  math_op));
	assert(register_entry(table, "XCHG", xchg));
}