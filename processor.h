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

typedef struct
{
    my_stack_t* stk1;
    int* buffer;
    size_t ip;
    int registers;
} my_pro_t;

int calculator(my_pro_t* stk1);

int* read_from_file(my_pro_t* str_pro, const char* TEST, size_t* size_of_buffer);

int ProcessorCtor(my_pro_t* str_pro);

int DumpPro (my_pro_t* str_pro,int i, const char* file, const char* func, int line);

#endif
