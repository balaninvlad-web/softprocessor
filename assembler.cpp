#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "enum.h"
#include "stack_functions.h"
#include "assembler.h"

void first_go (FILE* test, Labels* labels)
{
    char line[256] = {};
    int i = 0;

    printf("=== FIRST PASS ===\n");

    while (fgets(line, sizeof(line), test))
    {
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0 || line[0] == ';')
        {
            continue;
        }

        if (num_of_label(line))
        {
            int pos_labels = take_label_num(line);
            addlabels(pos_labels, i, labels);
        }
        else
        {
            i++;

            if (strstr(line, "PUSH") || strstr(line, "JMP") ||
                strstr(line, "JB") || strstr(line, "JBE") ||
                strstr(line, "JA") || strstr(line, "JAE") ||
                strstr(line, "JE") || strstr(line, "JNE") ||
                strstr(line, "PUSHREG") || strstr(line, "POPREG"))
            {
                i++;
            }
        }
    }
}

int main ()
{
    Labels labels;

    make_labels(&labels);

    my_stack_t* stack_call = (my_stack_t*)malloc(sizeof(my_stack_t));

    if (stack_call == NULL)
    {
        printf("Ошибка выделения памяти для стека вызовов\n");
        return -1;
    }
    StackCtor(stack_call, 100);

    FILE* test = fopen ("test2.ams", "r");

    if (test == NULL)
    {
        printf("Не удалось открыть файл test\n");
        return -1;
    }
    first_go(test, &labels);

    fseek(test, 0, SEEK_SET);

    size_t pos = 0;
    size_t capacity = 2000;

    int* code = (int*)calloc(capacity, sizeof(int));

    if (code == NULL)
    {
        printf("Ошибка: Не удалось выделить память\n");

        fclose(test);
        return -1;
    }

    char cmdStr[40] = "";

    while (1)
    {
        if(fscanf(test, "%s", cmdStr) == EOF)
        {
            break;
        }

        $_(pos);

        $(cmdStr);

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

            char argument[10] = {};

            fscanf(test, "%s", argument);

            if (argument[0] == ':')
            {
                int pos_labels = take_label_num(argument);
                printf("\n\nlabel: %d \n\n", pos_labels);
                int address = get_addres_label(pos_labels, &labels);
                if (address == -1)
                {
                    printf("ERROR: Label %s not found!\n", argument);
                    free(code);
                    fclose(test);
                    return -1;
                }
                code[pos++] = address;                                                                                                                          printf ("vvpdrs\n");

                printf("JMP to %s (address %d)\n", argument, address);
            }
            else
            {
                int arg = 0;

                fscanf(test, "%d", &arg);

                code[pos++] = arg;
            }
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

            char argument[10] = {};

            fscanf(test, "%s", argument);

            if (argument[0] == ':')
            {
                int pos_labels = take_label_num(argument);
                int address = get_addres_label(pos_labels, &labels);                if (address == -1)
                {
                    printf("ERROR: Label %s not found!\n", argument);
                    free(code);
                    fclose(test);
                    return -1;
                }
                code[pos++] = address;

                printf("JMP to %s (address %d)\n", argument, address);
            }
            else
            {
                int arg = 0;

                fscanf(test, "%d", &arg);

                code[pos++] = arg;
            }
        }
        else if (strcmp (cmdStr, "JBE") == 0)
        {
            code[pos++] = JBE;

            char argument[10] = {};

            fscanf(test, "%s", argument);

            if (argument[0] == ':')
            {
                int pos_labels = take_label_num(argument);
                int address = get_addres_label(pos_labels, &labels);
                if (address == -1)
                {
                    printf("ERROR: Label %s not found!\n", argument);
                    free(code);
                    fclose(test);
                    return -1;
                }
                code[pos++] = address;

                printf("JMP to %s (address %d)\n", argument, address);
            }
            else
            {
                int arg = 0;

                fscanf(test, "%d", &arg);

                code[pos++] = arg;
            }
        }
        else if (strcmp (cmdStr, "JA") == 0)
        {
            code[pos++] = JA;

            char argument[10] = {};

            fscanf(test, "%s", argument);

            if (argument[0] == ':')
            {
                int pos_labels = take_label_num(argument);
                int address = get_addres_label(pos_labels, &labels);
                if (address == -1)
                {
                    printf("ERROR: Label %s not found!\n", argument);
                    free(code);
                    fclose(test);
                    return -1;
                }
                code[pos++] = address;

                printf("JMP to %s (address %d)\n", argument, address);
            }
            else
            {
                int arg = 0;

                fscanf(test, "%d", &arg);

                code[pos++] = arg;
            }
        }
        else if (strcmp (cmdStr, "JAE") == 0)
        {
            code[pos++] = JAE;

            char argument[10] = {};

            fscanf(test, "%s", argument);

            if (argument[0] == ':')
            {
                int pos_labels = take_label_num(argument);
                int address = get_addres_label(pos_labels, &labels);
                if (address == -1)
                {
                    printf("ERROR: Label %s not found!\n", argument);
                    free(code);
                    fclose(test);
                    return -1;
                }
                code[pos++] = address;

                printf("JMP to %s (address %d)\n", argument, address);
            }
            else
            {
                int arg = 0;

                fscanf(test, "%d", &arg);

                code[pos++] = arg;
            }
        }
        else if (strcmp (cmdStr, "JE") == 0)
        {
            code[pos++] = JE;

            char argument[10] = {};

            fscanf(test, "%s", argument);

            if (argument[0] == ':')
            {
                int label_num = take_label_num(argument);
                int address = get_addres_label(label_num, &labels);
                if (address == -1)
                {
                    printf("ERROR: Label %s not found!\n", argument);
                    free(code);
                    fclose(test);
                    return -1;
                }
                code[pos++] = address;

                printf("JMP to %s (address %d)\n", argument, address);
            }
            else
            {
                int arg = 0;

                fscanf(test, "%d", &arg);

                code[pos++] = arg;
            }
        }
        else if (strcmp (cmdStr, "JNE") == 0)
        {
            code[pos++] = JNE;

            char argument[10] = {};

            fscanf(test, "%s", argument);

            if (argument[0] == ':')
            {
                int pos_labels = take_label_num(argument);
                int address = get_addres_label(pos_labels, &labels);
                if (address == -1)
                {
                    printf("ERROR: Label %s not found!\n", argument);
                    free(code);
                    fclose(test);
                    return -1;
                }
                code[pos++] = address;

                printf("JMP to %s (address: %d)\n", argument, address);
            }
            else
            {
                int arg = 0;

                fscanf(test, "%d", &arg);

                code[pos++] = arg;
            }
        }
        else if (strcmp (cmdStr, "POPREG") == 0)
        {
            code[pos++] = POPREG;
            printf("\n pop\n");
        }
        else if (strcmp (cmdStr, "INN") == 0)
        {
            code[pos++] = INN;
        }
        else if (strcmp (cmdStr, "CALL") == 0)
        {
            code[pos++] = CALL;

            char call_arg[10] = {};

            fscanf(test, "%s", call_arg);

            if (call_arg[0] == ':')
            {
                int pos_call = take_label_num(call_arg);
                int address = get_addres_label(pos_call, &labels);
                if (address == -1)
                {
                    printf("ERROR: CALL LABEL %s not found!\n", call_arg);
                    free(code);
                    fclose(test);
                    return -1;
                }
                code[pos++] = address;

                printf("CALL to %s (address: %d)\n", call_arg, address);
            }
            else
            {
                int arg = 0;

                fscanf(test, "%d", &arg);

                code[pos++] = arg;
            }
            pos++;

            StackPush(stack_call, pos);

            StackDump(stack_call, 0, __FILE__, __func__ ,__LINE__);
        }
        else if (strcmp (cmdStr, "RET"))
        {
            int ret_pos = 0;

            StackPop(stack_call, &ret_pos);

            pos = ret_pos;
        }
        else
            SyntaxERROR(pos, cmdStr);
    }

    //FILE* mashine_code = fopen("mashine_code.bin", "w");

    for (size_t i = 0; i < pos; i++)
    {
        printf("code[%d] = %d\n", i, (unsigned char)code[i]);
    }

    //printf("pos = %d\n",pos);
    //fclose(mashine_code);

    FILE* mashine_code_bin = fopen("mashine_code_bin5.bin", "wb");

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
    fclose(test);
    StackDtor(stack_call);
    free(stack_call);

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

void make_labels(Labels* labels)
{
    for (int i = 0; i < MAX_LABELS; i++)
    {
        labels->labels[i] = -1;
    }
}

void addlabels(int pos, int address, Labels* labels)
{
    if (pos >= 0 && pos < MAX_LABELS)
    {
        labels->labels[pos] = address;
        fprintf(stderr, "Label %d at address %d\n", pos, address);
    }
}

int get_addres_label (int pos_labels, Labels* labels)
{
    if (pos_labels >= 0 && pos_labels < MAX_LABELS)
    {
        return labels->labels[pos_labels];
    }
    return -1;
}

int num_of_label (const char* str)
{
    if (str[0] != ':')
        return 0;

    size_t len = strlen(str);

    for (int i = 1; i < len; i++)
    {
        if(isdigit(str[i]) == 0)
            return 0;
    }

    return 1;
}

int take_label_num (const char* label_str)
{
    return atoi(label_str + 1);
}
