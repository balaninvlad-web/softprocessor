//#define _TX_ALLOW_TRACE 9
#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "stack_functions.h"
#include "processor.h"

int main ()
{
    my_pro_t processor = {};

    if (ProcessorCtor(&processor) != NOERORR)
    {
        fprintf(stderr, "Failed to initialize processor\n");
        return ERORRPOINTER;
    }

    #ifdef DEBUG
        StackDump(processor.stk1, 0, __FILE__, __func__ ,__LINE__);
    #endif

    calculator(&processor);

    StackDtor(processor.stk1);
    free(processor.stk1);
    free(processor.buffer);

    return NOERORR;
}

