//#define _TX_ALLOW_TRACE 9
#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "stack_functions.h"
#include "processor.h"

int main (int argc,const char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <filename.bin>\n", argv[0]);
        return 1;
    }

    const char* file_name = argv[1];

    if (file_name == NULL)
    {
        fprintf(stderr, "ERROR: File name is NULL\n");
        return 1;
    }

    fprintf(stderr, "Running program: %s\n", file_name);

    my_pro_t processor;

    if (ProcessorCtor(&processor, file_name) != NOERORR)
    {
        fprintf(stderr, "Failed to initialize processor\n");
        return ERORRPOINTER;
    }

    #ifdef DEBUG
        StackDump(&processor.stk_for_calculate, 0, __FILE__, __func__ ,__LINE__);
    #endif

    Processor_calculate(&processor, file_name);

    ProcessorDtor(&processor);

    StackDtor(&processor.stk_for_calculate);
    free(processor.buffer);

    return NOERORR;
}

