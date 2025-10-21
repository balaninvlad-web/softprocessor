//#define _TX_ALLOW_TRACE 9
#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "stack_functions.h"
#include "processor.h"

int main (int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <filename.bin>\n", argv[0]);
        return 1;
    }

    char* file_name = argv[1];

    fprintf(stderr, "Running program: %s\n", file_name);

    my_pro_t processor = {};

    if (ProcessorCtor(&processor) != NOERORR)
    {
        fprintf(stderr, "Failed to initialize processor\n");
        return ERORRPOINTER;
    }

    #ifdef DEBUG
        StackDump(&processor.stk1, 0, __FILE__, __func__ ,__LINE__);
    #endif

    int results = Calculator(&processor, file_name);

    StackDtor(&processor.stk1);
    free(processor.buffer);

    return NOERORR;
}

