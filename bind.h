/**
 * @file: bind.h
 * @desc: Declares function that helps in binding instruction set to
 *        strings.
 */ 

#ifndef _ASE_BIND_H_
#define _ASE_BIND_H_

#include "flags.h"
#include "mathop.h"
#include "mem.h"
#include "stack.h"
#include "tengine.h"

void bind_calls(table_t *table);

#endif