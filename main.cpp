//#define _TX_ALLOW_TRACE 9
#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "stack_functions.h"
#include "calculator.h"

int main ()
{
    my_stack_t stk1 = {};
    StackCtor (&stk1, 10); // функция заполнения массива

    #ifdef DEBUG
        StackDump(&stk1, 0, __FILE__, __func__ ,__LINE__);
    #endif

    calculator (&stk1);

    StackDtor(&stk1);

    return NOERORR;
}

