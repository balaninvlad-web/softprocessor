#include "stack_functions.h"
#include "processor.h"

/*int* read_from_file(const char* TEST, size_t* size_of_buffer)
{
    FILE* input_file = fopen(TEST, "rb");

    assert(input_file != NULL);

    struct stat statbuf ={};

    stat (TEST, &statbuf);

    *size_of_buffer = statbuf.st_size;

    assert(*size_of_buffer != 0);

    $(*size_of_buffer);

    int* buffer = (int*)calloc(*size_of_buffer + 1, sizeof(char));

    assert(buffer != NULL);

    buffer[*size_of_buffer] = '\0';

    fread(buffer, sizeof(int), *size_of_buffer, input_file);

    assert(buffer != NULL);

    return buffer;
}*/

int calculator (my_pro_t* str_pro)
{
    const char* TEST = "mashine_code_bin4.bin";
    int* buffer = 0;
    size_t size_of_buffer = 0;

    buffer = read_from_file(str_pro, TEST, &size_of_buffer);

    int value_cal = 0;
    int command = 0;
    str_pro->ip = 0;
    int A, B, C, D = 0;

    assert(str_pro->stk1->data != NULL);

    while (command != HTL)
    {
        command = buffer[str_pro->ip];

        #ifdef DEBUG
            $_(str_pro->ip);
            $(command);
        #endif

        str_pro->ip++;

        if (command == PUSH)
        {
            value_cal = buffer[str_pro->ip];
            #ifdef DEBUG
                fprintf(stderr, "PUSHED VALUE: %d\n",value_cal);
            #endif

            str_pro->ip++;

            StackPush(str_pro->stk1, value_cal);
            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == INN)
        {
            value_cal = 0;

            fprintf(stderr,"INN VALUE: ");

            scanf("%d", &value_cal);

            #ifdef DEBUG
                fprintf(stderr, "PUSHED VALUE: %d\n",value_cal);
            #endif

            str_pro->ip++;

            StackPush(str_pro->stk1, value_cal);
            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == ADD)
        {
            int x = 0;
            int y = 0;

            StackPop(str_pro->stk1, &x);
            StackPop(str_pro->stk1, &y);
            #ifdef DEBUG
                fprintf(stderr, "popeded numbers:\n%d\n", x);
                fprintf(stderr, "%d\n", y);
            #endif

            int s = x + y;

            StackPush(str_pro->stk1, s);

            #ifdef DEBUG
                fprintf(stderr,"\n\npushed value: %d\n\n", s);
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == SUB)
        {
            int x = 0;
            int y = 0;

            StackPop(str_pro->stk1, &x);
            StackPop(str_pro->stk1, &y);
            #ifdef DEBUG
                fprintf(stderr, "popeded numbers:\n%d\n %d\n", x, y);
            #endif

            int s = y - x;

            StackPush(str_pro->stk1, s);

            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == MUL)
        {
            int x = 0;
            int y = 0;

            StackPop(str_pro->stk1, &x);
            StackPop(str_pro->stk1, &y);

            #ifdef DEBUG
                fprintf(stderr, "MUL values is:\n%d\n %d\n", x, y);
            #endif

            int s = x * y;

            StackPush(str_pro->stk1, s);

            printf("%d\n",s);

            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == DIV)
        {
            int x = 0;
            int y = 0;

            StackPop(str_pro->stk1, &x);
            StackPop(str_pro->stk1, &y);
            #ifdef DEBUG
                fprintf(stderr, "%d\n %d\n", x, y);
            #endif

            int s = y / x;

            StackPush(str_pro->stk1, s);

            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == OUTP)
        {
            int x = 0;

            StackPop(str_pro->stk1, &x);

            printf("\nOUT VALUE: %d\n", x);

            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == POW) // TODO: vozvedenie v stepen'
        {
            int x = 0;

            StackPop(str_pro->stk1, &x);

            #ifdef DEBUG
                fprintf(stderr,"%d\n", x);
            #endif

            int s = x * x;
            #ifdef DEBUG
                fprintf(stderr, "\n\nPOW value: %d\n", s);
            #endif

            StackPush(str_pro->stk1, s);

            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == SQRT)
        {
            int x = 0;

            StackPop(str_pro->stk1, &x);

            #ifdef DEBUG
                fprintf(stderr, "sqrt of:  %d\n", x);
            #endif

            int s = (int) sqrt((double) x);

            StackPush(str_pro->stk1, s);

            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == PUSHREG)
        {
            command = buffer[str_pro->ip];

            if (command == Ax)
            {
            // QUE: если пишем PUSHREG мы присваиваем и переменной и кладем в стек?
            #ifdef DEBUG
                fprintf(stderr, "PUSHED AX : %d\n",A);
            #endif

            StackPush(str_pro->stk1, A);
            }

            if (command == Bx)
            {
            #ifdef DEBUG
                fprintf(stderr, "PUSHED BX : %d\n",B);
            #endif

            StackPush(str_pro->stk1, B);
            }

            if (command == Cx)
            {
            #ifdef DEBUG
                fprintf(stderr, "PUSHED CX : %d\n",C);
            #endif

            StackPush(str_pro->stk1, C);
            }

            if (command == Dx)
            {
            #ifdef DEBUG
                fprintf(stderr, "PUSHED DX : %d\n",D);
            #endif

            StackPush(str_pro->stk1, D);
            }


            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }
        if (command == POPREG)
        {
            command = buffer[str_pro->ip];

            if (command == Ax)
            {
            StackPop(str_pro->stk1, &A);
            #ifdef DEBUG
                fprintf(stderr, "POPED AX : %d\n",A);
            #endif

            }

            if (command == Bx)
            {
            StackPop(str_pro->stk1, &B);
            #ifdef DEBUG
                fprintf(stderr, "POPED BX : %d\n",B);
            #endif
            }

            if (command == Cx)
            {
            StackPop(str_pro->stk1, &C);
            #ifdef DEBUG
                fprintf(stderr, "POPED CX : %d\n",C);
            #endif
            }

            if (command == Dx)
            {
            StackPop(str_pro->stk1, &D);
            #ifdef DEBUG
                fprintf(stderr, "POPED DX : %d\n",D);
            #endif
            }


            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }
        if (command == POP)
        {
            int x = 0;

            StackPop(str_pro->stk1, &x);

            fprintf(stderr, "%d", x);

            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == JMP)
        {
            value_cal = buffer[str_pro->ip];
            #ifdef DEBUG
                fprintf(stderr, "STR JUMP: %d\n",value_cal);
            #endif
            str_pro->ip = value_cal;

            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
            #endif
        }

        if (command == JB)
        {

            int x = 0;
            int y = 0;

            StackPop(str_pro->stk1, &x);
            StackPop(str_pro->stk1, &y);
            $_(x);
            $_(y);

            if (y > x)
            {
                value_cal = buffer[str_pro->ip];
                #ifdef DEBUG
                    fprintf(stderr, "STR JUMP: %d\n",value_cal);
                #endif
                str_pro->ip = value_cal;

                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
                #endif
            }
            else
                str_pro->ip++;
        }

        if (command == JBE)
        {

            int x = 0;
            int y = 0;

            StackPop(str_pro->stk1, &x);
            StackPop(str_pro->stk1, &y);

            if (y >= x)
            {
                value_cal = buffer[str_pro->ip];
                #ifdef DEBUG
                    fprintf(stderr, "STR JUMP: %d\n",value_cal);
                #endif
                str_pro->ip = value_cal;

                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
                #endif
            }
            else
                str_pro->ip++;
        }

        if (command == JA)
        {

            int x = 0;
            int y = 0;

            StackPop(str_pro->stk1, &x);
            StackPop(str_pro->stk1, &y);

            if (y < x)
            {
                value_cal = buffer[str_pro->ip];
                #ifdef DEBUG
                    fprintf(stderr, "STR JUMP: %d\n",value_cal);
                #endif
                str_pro->ip = value_cal;

                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
                #endif
            }
            else
                str_pro->ip++;
        }

        if (command == JAE)
        {

            int x = 0;
            int y = 0;

            StackPop(str_pro->stk1, &x);
            StackPop(str_pro->stk1, &y);

            if (y <= x)
            {
                value_cal = buffer[str_pro->ip];
                #ifdef DEBUG
                    fprintf(stderr, "STR JUMP: %d\n",value_cal);
                #endif
                str_pro->ip = value_cal;

                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
                #endif
            }
            else
                str_pro->ip++;
        }

        if (command == JE)
        {

            int x = 0;
            int y = 0;

            StackPop(str_pro->stk1, &x);
            StackPop(str_pro->stk1, &y);

            if (y == x)
            {
                value_cal = buffer[str_pro->ip];
                #ifdef DEBUG
                    fprintf(stderr, "STR JUMP: %d\n",value_cal);
                #endif
                str_pro->ip = value_cal;

                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
                #endif
            }
            else
                str_pro->ip++;
        }

        if (command == JNE)
        {

            int x = 0;
            int y = 0;

            StackPop(str_pro->stk1, &x);
            StackPop(str_pro->stk1, &y);

            if (y != x)
            {
                value_cal = buffer[str_pro->ip];
                #ifdef DEBUG
                    fprintf(stderr, "STR JUMP: %d\n",value_cal);
                #endif
                str_pro->ip = value_cal;

                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__);
                #endif
            }
            else
                str_pro->ip++;
        }

    }
    return 0;
}
