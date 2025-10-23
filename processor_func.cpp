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

int* read_from_file(my_pro_t* str_pro, const char* test, size_t* size_of_buffer)
{
    fprintf(stderr, "TEST PRO :%s\n", test);

    FILE* input_file = fopen(test, "rb");

    assert(input_file != NULL);

    struct stat statbuf ={};

    stat (test, &statbuf);

    *size_of_buffer = statbuf.st_size;

    assert(*size_of_buffer != 0);

    $(*size_of_buffer);

    str_pro->buffer = (int*)calloc(*size_of_buffer + 1, sizeof(char));

    str_pro->buffer[*size_of_buffer] = '\0';

    fread(str_pro->buffer, sizeof(int), *size_of_buffer, input_file);

    #ifdef DEBUG
        for (size_t i = 0; i < *size_of_buffer/4 ; i++)
        {
            fprintf(stderr, "%d |i = %d|\n",str_pro->buffer[i], i);
        }
    #endif

    return str_pro->buffer;
}

int ProcessorCtor (my_pro_t* str_pro, const char* file_name)
{
    assert(str_pro);

    size_t size_of_buffer = 0;

    fprintf(stderr, "Loading file: %s\n", file_name);

    const char* test = file_name;

    fprintf(stderr, "Loading file test: %s\n", test);

    if (StackCtor(&str_pro->stk_for_calculate, 6) != NOERORR) //TODO переименновать
    {
        return ERORRDATANULL;
    }

    if (StackCtor(&str_pro->stk_call, 6) != NOERORR)
    {
        return ERORRDATANULL;
    }

    str_pro->buffer = read_from_file(str_pro, test, &size_of_buffer);

    str_pro->ip = 0;

    for(size_t i = 0; i <= 4; i++)
        str_pro->registers[i] = 0;

    str_pro->Ram_Mem [number_of_mem_cells] ={}; // TODO убрать число

    #ifdef DEBUG
        verificator_pro (str_pro, __FILE__, __func__ ,__LINE__, -1);
    #endif

    return NOERORR;
}

void ProcessorDtor(my_pro_t* str_pro)
{
    assert(str_pro != NULL);

    if (str_pro->stk_for_calculate.data != NULL)
    {
        StackDtor(&str_pro->stk_for_calculate);
    }

    if (str_pro->stk_call.data != NULL)
    {
        StackDtor(&str_pro->stk_call);
    }

    free(str_pro->buffer);
    // TODO освобождать буффер
}

int verificator_pro (my_pro_t* str_pro, const char* file, const char* func, int line, int command)
{
    int i = 0;
    if (str_pro->buffer == NULL)
    {
        i |= ERORRNULLBUFF;
    }
    if (i > 0)
    {
        DumpPro(str_pro, i, file, func, line, command);
    }
    return i;
}

int DumpPro (my_pro_t* str_pro,int i, const char* file, const char* func, int line, int command)
{
    fprintf(stderr, "----------------------------DUMBPROCESSOR\n\n");

    fprintf(stderr, "code: %d\n", command);

    fprintf(stderr, "pos ip: %lu\n", (unsigned long)str_pro->ip);

    fprintf(stderr, "called in file: %s, function: %s, in line: %d", file, func, line);

    fprintf(stderr, "[%p]\n", (void*)str_pro);

    fprintf(stderr, "[Ax] = %d  [Bx] = %d  [Cx] = %d  [Dx] = %d\n\n", str_pro->registers[POS_Ax], str_pro->registers[POS_Bx], str_pro->registers[POS_Cx], str_pro->registers[POS_Dx]);

    fprintf(stderr, "RAM_mem:\n");

    for(int add_mem = 1; add_mem < 100; add_mem++)
    {
        fprintf(stderr, "%4d", str_pro->Ram_Mem[add_mem]);

        if (add_mem % 10 == 0)
            printf("\n");
    }

    printf("\n");

    verificator (&str_pro->stk_for_calculate,  file, func, line);

    StackDump(&str_pro->stk_for_calculate, i, file, func, line);

    fprintf(stderr, "----------------------------DUMBPROCESSOREND\n\n");

    return 0;
}
