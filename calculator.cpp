#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include "stack_functions.h"
#include "calculator.h"

int* read_from_file(const char* TEST, size_t size_of_buffer); //TODO в ашку засунь

int* read_from_file(const char* TEST, size_t size_of_buffer)
{
    FILE* input_file = fopen(TEST, "rb");

    assert(input_file != NULL);

    struct stat statbuf ={};

    stat (TEST, &statbuf);

    size_of_buffer = statbuf.st_size;

    assert(size_of_buffer != 0);

    $(size_of_buffer);

    int* buffer = (int*)calloc(size_of_buffer + 1, sizeof(char));

    assert(buffer != NULL);

    buffer[size_of_buffer] = '\0';

    fread(buffer, sizeof(int), size_of_buffer, input_file);

    assert(buffer != NULL);

    return buffer;
}

int calculator (my_stack_t* stk1)
{

    const char* TEST = "mashine_code_bin.bin";
    int* buffer = 0;
    size_t size_of_buffer = 0;

    buffer = read_from_file(TEST, size_of_buffer);

    int value_cal = 0;
    int command = 0;
    size_t ip = 0;

    while (command != HTL)
    {
        command = buffer[ip];

        ip++;

        if (command == PUSH)
        {
            value_cal = buffer[ip];// TODO чтение из массива

            fprintf(stderr, "PUSHED VALUE: %d\n",value_cal);

            ip++;

            StackPush(stk1, value_cal);

            #ifdef DEBUG
                StackDump(stk1, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == ADD)
        {
            int x = 0;
            int y = 0;

            StackPop(stk1, &x);
            StackPop(stk1, &y);
            #ifdef DEBUG
                fprintf(stderr, "popeded numbers:\n%d\n", x);
                fprintf(stderr, "%d\n", y);
            #endif

            int s = x + y;

            StackPush(stk1, s);

            #ifdef DEBUG
                fprintf(stderr,"\n\npushed value: %d\n\n", s);
                StackDump(stk1, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == SUB)
        {
            int x = 0;
            int y = 0;

            StackPop(stk1, &x);
            StackPop(stk1, &y);
            #ifdef DEBUG
                fprintf(stderr, "popeded numbers:\n%d\n %d\n", x, y);
            #endif

            int s = y - x;

            StackPush(stk1, s);

            #ifdef DEBUG
                StackDump(stk1, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == MUL)
        {
            int x = 0;
            int y = 0;

            StackPop(stk1, &x);
            StackPop(stk1, &y);

            #ifdef DEBUG
                fprintf(stderr, "MUL values is:\n%d\n %d\n", x, y);
            #endif

            int s = x * y;

            StackPush(stk1, s);

            printf("%d\n",s);

            #ifdef DEBUG
                StackDump(stk1, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == DIV)
        {
            int x = 0;
            int y = 0;

            StackPop(stk1, &x);
            StackPop(stk1, &y);
            #ifdef DEBUG
                fprintf(stderr, "%d\n %d\n", x, y);
            #endif

            int s = y / x;

            StackPush(stk1, s);

            #ifdef DEBUG
                StackDump(stk1, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == OUTP)
        {
            int x = 0;

            StackPop(stk1, &x);

            printf("\nOUT VALUE: %d\n", x);

            #ifdef DEBUG
                StackDump(stk1, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == POW) // TODO: vozvedenie v stepen'
        {
            int x = 0;

            StackPop(stk1, &x);

            #ifdef DEBUG
                fprintf(stderr,"%d\n", x);
            #endif

            int s = x * x;

            StackPush(stk1, s);

            #ifdef DEBUG
                StackDump(stk1, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == SQvRT)
        {
            int x = 0;

            StackPop(stk1, &x);

            #ifdef DEBUG
                fprintf(stderr, "sqrt of:  %d\n", x);
            #endif

            int s = (int) sqrt((double) x);

            StackPush(stk1, s);

            #ifdef DEBUG
                StackDump(stk1, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

    }
    return 0;
}
