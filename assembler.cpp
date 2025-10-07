#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "enum.h"

int SyntaxERROR(int pos, char* smdStr);

int main ()
{
    size_t pos = 0;
    size_t capacity = 100;

    FILE* test = fopen ("test2.ams", "r");

    if (test == NULL)
    {
        printf("Не удалось открыть файл test\n");
        return -1;
    }

    int* code = (int*)calloc(capacity, sizeof(int));

    if (code == NULL)
    {
        printf("Ошибка: Не удалось выделить память\n");

        fclose(test);
        return -1;
    }

    char cmdStr[40] = "";

    while (1)// пока не конец файла
    {
        if(fscanf(test, "%39s", cmdStr) == EOF)
        {
            break;
        }

        //$_(pos);

        //$(cmdStr);

        if (pos + 2 >= capacity)
        {
            capacity *= 2;
            int* new_code = (int*)realloc(code, capacity);

            if (new_code == NULL)
            {
                printf("Не удалось перевыделить память\n");
                free(code);
                fclose(test);
                return -1;
            }

        code = new_code;
        }
        if (strcmp (cmdStr, "PUSH") == 0)
        {
            code[pos++] = PUSH;

            int arg = 0;

            fscanf(test, "%d", &arg);

            code[pos++] = arg;
        }
        else if (strcmp (cmdStr, "ADD") == 0)
        {
            code[pos++] = ADD;
        }
        else if (strcmp (cmdStr, "SUB") == 0)
        {
            code[pos++] = SUB;
        }
        else if (strcmp (cmdStr, "DIV") == 0)
        {
            code[pos++] = DIV;
        }
        else if (strcmp (cmdStr, "OUT") == 0)
        {
            code[pos++] = OUTP;
        }
        else if (strcmp (cmdStr, "SQRT") == 0)
        {
            code[pos++] = SQRT;
        }
        else if (strcmp (cmdStr, "HTL") == 0)
        {
            code[pos++] = HTL;
        }
        else if (strcmp (cmdStr, "MUL") == 0)
        {
            code[pos++] = MUL;
        }
        else if (strcmp (cmdStr, "POP") == 0)
        {
            code[pos++] = POP;
        }
        else if (strcmp (cmdStr, "JMP") == 0)
        {
            code[pos++] = JMP;

            int arg = 0;

            fscanf(test, "%d", &arg);

            code[pos++] = arg;
        }
        else if (strcmp (cmdStr, "POW") == 0)
        {
            code[pos++] = POW;
        }
        else if (strcmp (cmdStr, "PUSHREG") == 0)
        {
            code[pos++] = PUSHREG;
            printf("\n push\n");
        }
        else if (strcmp (cmdStr, "Ax") == 0)
        {
            code[pos++] = Ax;
        }
        else if (strcmp (cmdStr, "Bx") == 0)
        {
            code[pos++] = Bx;
        }
        else if (strcmp (cmdStr, "Cx") == 0)
        {
            code[pos++] = Cx;
        }
        else if (strcmp (cmdStr, "Dx") == 0)
        {
            code[pos++] = Dx;
        }
        else if (strcmp (cmdStr, "JB") == 0)
        {
            code[pos++] = JB;

            int arg = 0;

            fscanf(test, "%d", &arg);

            code[pos++] = arg;
        }
        else if (strcmp (cmdStr, "JBE") == 0)
        {
            code[pos++] = JBE;

            int arg = 0;

            fscanf(test, "%d", &arg);

            code[pos++] = arg;
        }
        else if (strcmp (cmdStr, "JA") == 0)
        {
            code[pos++] = JA;

            int arg = 0;

            fscanf(test, "%d", &arg);

            code[pos++] = arg;
        }
        else if (strcmp (cmdStr, "JAE") == 0)
        {
            code[pos++] = JAE;

            int arg = 0;

            fscanf(test, "%d", &arg);

            code[pos++] = arg;
        }
        else if (strcmp (cmdStr, "JE") == 0)
        {
            code[pos++] = JE;

            int arg = 0;

            fscanf(test, "%d", &arg);

            code[pos++] = arg;
        }
        else if (strcmp (cmdStr, "JNE") == 0)
        {
            code[pos++] = JNE;

            int arg = 0;

            fscanf(test, "%d", &arg);

            code[pos++] = arg;
        }
        else if (strcmp (cmdStr, "POPREG") == 0)
        {
            code[pos++] = POPREG;
            printf("\n pop\n");
        }
        if (strcmp (cmdStr, "INN") == 0)
        {
            code[pos++] = INN;
        }
        //else
           // SyntaxERROR(pos, cmdStr);
    }

    //FILE* mashine_code = fopen("mashine_code.bin", "w");

    for (size_t i = 0; i < pos; i++)
    {
        printf("code[%d] = %d\n", i, (unsigned char)code[i]);
    }

    //printf("pos = %d\n",pos);
    //fclose(mashine_code);

    FILE* mashine_code_bin = fopen("mashine_code_bin4.bin", "wb");

    if (mashine_code_bin != NULL)
    {

        fwrite(code, sizeof(int), pos, mashine_code_bin);
        fclose(mashine_code_bin);
        printf("записано в бинарный файл\n");
    }
    else
    {
        printf("ошибка перезаписи fwrite");
    }

    free(code);

    return 0;
}


int SyntaxERROR(int pos, char* cmdStr)
{
    $r
    printf("\n\nERORR in command: %s\n",cmdStr);
    printf("ERORR in assembler, in position %d\n\n", pos);
    $d
    return -1;
}
