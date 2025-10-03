#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include "stack_functions.h"
#include "enum.h"

int calculator(my_stack_t* stk1);

int* read_from_file(const char* TEST, size_t* size_of_buffer);

#endif
