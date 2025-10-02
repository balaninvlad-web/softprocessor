#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "enum.h"

int SyntaxERROR(int pos);

int main ()
{
    size_t pos = 0;
    size_t capacity = 1000;

    FILE* test = fopen ("test.ams", "r");

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

    char cmdStr[20] = "";

    while (fscanf(test, "%19s", cmdStr) != EOF)// пока не конец файла
    {
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

            char arg = 0;

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
        else if (strcmp (cmdStr, "SQvRT") == 0)
        {
            code[pos++] = SQvRT;
        }
        else if (strcmp (cmdStr, "HTL") == 0)
        {
            code[pos++] = HTL;
        }
        else if (strcmp (cmdStr, "MUL") == 0)
        {
            code[pos++] = MUL;
        }
        else
            SyntaxERROR(pos);
    }

    //FILE* mashine_code = fopen("mashine_code.bin", "w");

    for (size_t i = 0; i < pos; i++)
    {
        printf("code[%zu] = %d\n", i, (unsigned char)code[i]);
    }

    printf("pos = %zu\n",pos);
    //fclose(mashine_code);

    FILE* mashine_code_bin = fopen("mashine_code_bin.bin", "wb");

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


int SyntaxERROR(int pos)
{
    printf("ERORR in assembler, in position %d", pos);

    return -1;
}
