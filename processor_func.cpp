#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include "processor.h"

#define ERORRNULLBUFF 1
#define ERORRNULLSTK1 2

int verificator_pro(my_pro_t* str_pro, const char* file, const char* func, int line);
//int DumpPro(my_pro_t* str_pro, int i, const char* file, const char* func, int line);
//int ProcessorCtor(my_pro_t* str_pro);

int* read_from_file(my_pro_t* str_pro, const char* TEST, size_t* size_of_buffer)
{
    FILE* input_file = fopen(TEST, "rb");

    assert(input_file != NULL);

    struct stat statbuf ={};

    stat (TEST, &statbuf);

    *size_of_buffer = statbuf.st_size;

    assert(*size_of_buffer != 0);

    $(*size_of_buffer);

    str_pro->buffer = (int*)calloc(*size_of_buffer + 1, sizeof(char));

    str_pro->buffer[*size_of_buffer] = '\0';

    fread(str_pro->buffer, sizeof(int), *size_of_buffer, input_file);

    return str_pro->buffer;
}

int ProcessorCtor (my_pro_t* str_pro)
{
    assert(str_pro);

    size_t size_of_buffer = 0;
    const char* TEST = "mashine_code_bin4.bin";

    str_pro->stk1 = (my_stack_t*)malloc(sizeof(my_stack_t));
    if (str_pro->stk1 == NULL) {
        return ERORRDATANULL;
    }

    if (StackCtor(str_pro->stk1, 6) != NOERORR) {
        free(str_pro->stk1);
        return ERORRDATANULL;
    }

    str_pro->buffer = read_from_file(str_pro, TEST, &size_of_buffer);
    str_pro->ip = 0;

    #ifdef DEBUG
        verificator_pro (str_pro, __FILE__, __func__ ,__LINE__);
    #endif


    return NOERORR;
}

int verificator_pro (my_pro_t* str_pro, const char* file, const char* func, int line)
{
    int i = 0;
    if (str_pro->buffer == NULL)
    {
        i |= ERORRNULLBUFF;
    }
    if (str_pro->stk1 != NULL)
    {
        i |= ERORRNULLSTK1;
    }
    if (i > 0)
    {
        DumpPro(str_pro, i, file, func, line);
    }
    return i;
}

int DumpPro (my_pro_t* str_pro,int i, const char* file, const char* func, int line)
{
    fprintf(stderr, "----------------------------DUMBPROCESSOR\n\n");

    fprintf(stderr, "pos ip: %lu\n", (unsigned long)str_pro->ip);

    fprintf(stderr, "called in file: %s, function: %s, in line: %d", file, func, line);

    fprintf(stderr, "[%p]\n", (void*)str_pro);

    verificator (str_pro->stk1,  file, func, line);

    StackDump(str_pro->stk1, i, file, func, line);

    fprintf(stderr, "----------------------------DUMBPROCESSOREND\n\n");

    return 0;
}
