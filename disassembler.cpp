#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "enum.h"

#include "stack_functions.h"
#include "processor.h"

int SyntaxERROR(int pos);

int main ()
{
    my_pro_t str_pro2 = {};

    const char* TEST = "mashine_code_bin5.bin";

    if (TEST == NULL)
    {
        fprintf(stderr, "Не удалось открыть файл mashine_code_bin\n");
        return -1;
    }

    int* buffer = 0;
    size_t size_of_buffer = 0;

    buffer = read_from_file(&str_pro2, TEST, &size_of_buffer);

    size_of_buffer = size_of_buffer/ sizeof(int);

    size_t pos = 0;
    size_t capacity = 100;

    FILE* MASHINCODE = fopen("recoded_code.txt", "w");

    char cmdStr[20] = "";

    //TODO: printf command function

    while (pos <= size_of_buffer)// пока не конец файла
    {
        if (buffer[pos] == PUSH)
        {
            fprintf(MASHINCODE, "PUSH ");

            fprintf(MASHINCODE, "%d\n", buffer[++pos]);
        }

        else if (buffer[pos] == ADD)
        {
            fprintf (MASHINCODE, "ADD\n");
        }
        else if (buffer[pos] == SUB)
        {
            fprintf (MASHINCODE, "SUB\n");
        }
        else if (buffer[pos] == DIV)
        {
            fprintf (MASHINCODE, "DIV\n");
        }
        else if (buffer[pos] == OUTP)
        {
            fprintf (MASHINCODE, "OUT\n");
        }
        else if (buffer[pos] == SQRT)
        {
            fprintf (MASHINCODE, "SQRT\n");
        }
        else if (buffer[pos] == HTL)
        {
            fprintf (MASHINCODE, "HTL\n");
        }
        else if (buffer[pos] == MUL)
        {
            fprintf (MASHINCODE, "MUL\n");
        }
        else if (buffer[pos] == POP)
        {
            fprintf (MASHINCODE, "POP\n");
        }
        else if (buffer[pos] == JMP)
        {
            fprintf (MASHINCODE, "JMP\n");
        }
        else if (buffer[pos] == PUSHREG)
        {
            fprintf (MASHINCODE, "PUSHREG ");
        }
        else if (buffer[pos] == Ax)
        {
            fprintf (MASHINCODE, "Ax\n");
        }
        else if (buffer[pos] == Bx)
        {
            fprintf (MASHINCODE, "Bx\n");
        }
        else if (buffer[pos] == Cx)
        {
            fprintf (MASHINCODE, "Cx\n");
        }
        else if (buffer[pos] == Dx)
        {
            fprintf (MASHINCODE, "Dx\n");
        }
        else if (buffer[pos] == POPREG)
        {
            fprintf (MASHINCODE, "POPREG ");
        }
        else if (buffer[pos] == JB)
        {
            fprintf (MASHINCODE, "JB ");
        }
        else if (buffer[pos] == JBE)
        {
            fprintf (MASHINCODE, "JBE ");
        }
        else if (buffer[pos] == JA)
        {
            fprintf (MASHINCODE, "JA ");
        }
        else if (buffer[pos] == JAE)
        {
            fprintf (MASHINCODE, "JAE ");
        }
        else if (buffer[pos] == JE)
        {
            fprintf (MASHINCODE, "JE ");
        }
        else if (buffer[pos] == JNE)
        {
            fprintf (MASHINCODE, "JNE ");
        }
        else if (buffer[pos] == INN)
        {
            fprintf (MASHINCODE, "INN");
        }
        else
            fprintf (MASHINCODE, "%d\n", buffer[pos]);

        pos++;
    }

    //FILE* mashine_code = fopen("mashine_code.bin", "w");

    for (size_t i = 0; i < size_of_buffer; i++)
    {
        printf("buffer[%zu] = %d\n", i, (unsigned char)buffer[i]);
    }
    //fclose(mashine_code);

/*    FILE* mashine_code_bin = fopen("mashine_code_bin.bin", "wb");

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

    return 0;*/
}

int* read_from_file(const char* TEST, size_t* size_of_buffer)
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
}

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

    #ifdef DEBUG
        for (int i = 0; i < *size_of_buffer/4 ; i++)
        {
            fprintf(stderr, "%d |i = %d|\n",str_pro->buffer[i], i);
        }
    #endif

    return str_pro->buffer;
}

int SyntaxERROR(int pos)
{
    printf("ERORR in assembler, in position %d\n", pos);

    return -1;
}
