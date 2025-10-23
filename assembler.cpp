#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "enum.h"
#include "stack_functions.h"
#include "assembler.h"

StructCmd command_table [MAX_COUNT_COMMAND] = {
{ "PUSH"    , PUSH      , value   },//TODO указатель на функцию
{ "OUT"     , OUTP      , no_arg  },//TODO указатель на функцию
{ "ADD"     , ADD       , no_arg  },//TODO указатель на функцию
{ "SUB"     , SUB       , no_arg  },//TODO указатель на функцию
{ "DIV"     , DIV       , no_arg  },//TODO указатель на функцию
{ "OUTP"    , OUTP      , no_arg  },//TODO указатель на функцию
{ "SQRT"    , SQRT      , no_arg  },//TODO указатель на функцию
{ "MUL"     , MUL       , no_arg  },//TODO указатель на функцию
{ "POW"     , POW       , no_arg  },//TODO указатель на функцию
{ "JMP"     , JMP       , JMP_arg },//TODO указатель на функцию
{ "HTL"     , HTL       , no_arg  },//TODO указатель на функцию
{ "POP"     , POP       , no_arg  },//TODO указатель на функцию
{ "MOD"     , MOD       , no_arg  },//TODO указатель на функцию
{ "PUSHREG" , PUSHREG   , reg_val },//TODO указатель на функцию
{ "POPREG"  , POPREG    , reg_val },//TODO указатель на функцию
{ "JB"      , JB        , JMP_arg },//TODO указатель на функцию
{ "JA"      , JA        , JMP_arg },//TODO указатель на функцию
{ "JAE"     , JAE       , JMP_arg },//TODO указатель на функцию
{ "JBE"     , JBE        , JMP_arg},//TODO указатель на функцию
{ "JE"      , JE        , JMP_arg },//TODO указатель на функцию
{ "JNE"     , JNE       , JMP_arg },//TODO указатель на функцию
{ "INN"     , INN       , no_arg  },//TODO указатель на функцию
{ "RET"     , RET       , no_arg  },//TODO указатель на функцию
{ "CALL"    , CALL      , call_arg},//TODO указатель на функцию
{ "PUSHM"   , PUSHM     , mem_arg },//TODO указатель на функцию
{ "POPM"    , POPM      , mem_arg },//TODO указатель на функцию
{ "DISP"    , DISP      , no_arg  } //TODO указатель на функцию
};

void first_go (FILE* file_asm, Labels* labels)
{
    char line[256] = {};
    int i = 0;

    printf("=== FIRST PASS ===\n");

    while (fgets(line, sizeof(line), file_asm))
    {
        line[strcspn(line, "\n")] = 0;

        char* comment_pos = strchr(line, ';');
        if (comment_pos != NULL) {
            *comment_pos = '\0';
        }

        char *start = line;
        while (isspace(*start)) start++;

        char *end = start + strlen(start) - 1;
        while (end > start && isspace(*end)) end--;
        *(end + 1) = '\0';

        if (strlen(start) == 0)
        {
            continue;
        }

        if (is_label(start))
        {
            int pos_labels = take_label_num(start);
            add_labels(pos_labels, i, labels);
            printf("Label %d at address %d\n", pos_labels, i);
        }
        else
        {
            int command_size = 1;
            if ((strstr(start, "PUSH") == start) ||
                (strstr(start, "JMP") == start) ||
                (strstr(start, "JB") == start) ||
                (strstr(start, "JA") == start) ||
                (strstr(start, "JAE") == start) ||
                (strstr(start, "JE") == start) ||
                (strstr(start, "JNE") == start) ||
                (strstr(start, "PUSHREG") == start) ||
                (strstr(start, "POPREG") == start) ||
                (strstr(start, "CALL") == start) ||
                (strstr(start, "PUSHM") == start) ||
                (strstr(start, "POPM") == start))
            {
                command_size = 2;
            }

            printf("Command '%s' size %d at position %d\n", start, command_size, i);

            i += command_size;
        }
    }
    printf("\n\n===END OF FIRST PASS\n");
}

int main (int argc, char* argv[])
{
    Labels labels = {};

    make_labels(&labels);

    //TODO: constant for calloc

    my_stack_t* stack_call = (my_stack_t*)calloc(10000, sizeof(my_stack_t));

    if (stack_call == NULL)
    {
        printf("Ошибка выделения памяти для стека вызовов\n");
        return -1;
    }

    StackCtor(stack_call, 100);

    //char filename[100] = "";
    //printf("Введите имя asm файла: ");
    //scanf("%99s", filename);

    fprintf(stderr, "%s", argv[1]);

    FILE* file_asm = fopen (argv[1], "r");

    if (file_asm == NULL)
    {
        printf("Не удалось открыть файл file_asm\n");
        return -1;
    }

    char cmdStr[40] = "";

    first_go(file_asm, &labels);

    printf("Labels found:\n");

    for (int i = 0; i < MAX_LABELS; i++)
    {
        if (labels.labels[i] != -1)
        {
            printf("Label %d -> address %d\n", i, labels.labels[i]);
        }
    }

    fseek(file_asm, 0, SEEK_SET);

    int pos = 0;
    int capacity = 2000;

    int* code = (int*)calloc(capacity, sizeof(int));

    if (code == NULL)
    {
        printf("Ошибка: Не удалось выделить память\n");

        fclose(file_asm);
        return -1;
    }

    while (1)
    {
        if(fscanf(file_asm, "%s", cmdStr) == EOF)
        {
            break;
        }

        if (strlen(cmdStr) == 0 || cmdStr[0] == ';')
            continue;

        if (is_label(cmdStr))
        {
            printf("Skipping label: %s\n", cmdStr);
            continue;
        }

        $_(pos);

        $(cmdStr);

        if (pos + 2 >= capacity)
        {
            if (ExtendCode(&capacity, &code) == -1)
            {
                return -1;
            }

        }

        code = find_command(cmdStr, file_asm, code, &labels, &pos, command_table);

        //code = AssembleCommand (cmdStr, file_asm, code, &labels, &pos);

    }

    for (size_t i = 0; i < (unsigned)pos; i++)
    {
        printf("code[%d] = %d\n", i, (unsigned char)code[i]);
    }

    printf("pos = %d\n",pos);

    PrintToBinaryFile(code, pos, argv[2]);

    free(code);
    fclose(file_asm);
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
    assert (labels);

    for (int i = 0; i < MAX_LABELS; i++)
    {
        labels->labels[i] = -1;
    }
}

void add_labels(int pos, int address, Labels* labels)
{
    assert(labels);

    if (pos >= 0 && pos < MAX_LABELS)
    {
        labels->labels[pos] = address;
        fprintf(stderr, "Label %d at address %d\n", pos, address);
    }
}

int get_addres_label (int pos_labels, Labels* labels)
{
    assert(labels);

    if (pos_labels >= 0 && pos_labels < MAX_LABELS)
    {
        return labels->labels[pos_labels];
    }

    return -1;
}

int is_label (const char* str)
{
    if (str == NULL) return 0;

    const char *p = str;
    while (isspace(*p)) p++;

    if (p[0] != ':')
        return 0;

    for (size_t i = 1; i < strlen(p); i++)
    {
        if(!isdigit(p[i]))
            return 0;
    }

    return 1;
}

int take_label_num (const char* label_str)
{
    return atoi(label_str + 1);
}

int ExtendCode(int* capacity, int** code)
{
    *capacity *= 2;
            int* new_code = (int*)realloc(*code, (*capacity) * sizeof(int));

            if (new_code == NULL)
            {
                printf("Не удалось перевыделить память\n");
                free(*code);
                return -1;
            }

            *code = new_code;

    return 0;
}

int PrintToBinaryFile(int* code,int pos, const char* file_name)
{
    FILE* mashine_code_bin = fopen(file_name, "wb");

    fprintf(stderr, "TEST :%s\n", file_name);

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
    return 0;
}


int* find_command (char* cmdStr, FILE* file_asm, int* code, Labels* labels, int* pos_ptr, StructCmd* all_Cmd)
{
    if (cmdStr == NULL)
    {
        fprintf(stderr, "ERROR: cmdStr is NULL\n");
        *pos_ptr = *pos_ptr; // не меняем позицию
        return code;
    }

    if (all_Cmd == NULL)
    {
        fprintf(stderr, "ERROR: all_Cmd is NULL\n");
        return code;
    }

    assert(cmdStr);
    assert(code);
    assert(pos_ptr);
    assert(*pos_ptr >= 0);

    int pos = *pos_ptr;

    for (size_t i = 0; i < MAX_COUNT_COMMAND; i++)
    {
        if (all_Cmd[i].name == NULL)
        {
            fprintf(stderr, "WARNING: all_Cmd[%d].name is NULL\n", i);
            continue;
        }


        if (strcmp(cmdStr, all_Cmd[i].name) == 0) // сделать функцию берющую только номер команды
        {
            switch (all_Cmd[i].arg)
            {
                case no_arg:
                {
                    code[pos++] = all_Cmd[i].Cmd;
                    break;
                }
                case value:
                {
                    Command_with_value(code, all_Cmd, &pos, file_asm, i);
                    break;
                }
                case JMP_arg:
                {
                    Command_for_Jump(code, all_Cmd, &pos, file_asm, i, labels);
                    break;
                }
                case reg_val:
                {
                    Command_for_reg(code, all_Cmd, &pos, file_asm, i);
                    break;
                }
                case call_arg:
                {
                    Command_for_call(code, all_Cmd, &pos, file_asm, i, labels);
                    break;
                }
                case mem_arg:
                {
                    Command_for_mem(code, all_Cmd, &pos, file_asm, i);
                    break;
                }
                default:
                    fprintf(stderr, "Unknown argument type for command: %s\n", cmdStr);
                    break;
            }
            *pos_ptr = pos;
            return code;
        }
    }
    fprintf(stderr, "FATAL ERORR, NIGGA\n");
    SyntaxERROR(pos, cmdStr);
    *pos_ptr = pos;
    return code;
}

/*int* AssembleCommand (char* cmdStr, FILE* file_asm, int* code, Labels* labels, int* pos_ptr)
{
    int pos = *pos_ptr;

    if (strcmp (cmdStr, "PUSH") == 0)
    {
        code[pos++] = PUSH;

        int arg = 0;

        fscanf(file_asm, "%d", &arg);

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

        printf("HTLLLLLL\n");
    }
    else if (strcmp (cmdStr, "RET") == 0)
    {
        printf("penis\n");
        code[pos++] = RET;
    }
    else if (strcmp (cmdStr, "MUL") == 0)
    {
        code[pos++] = MUL;
    }
    else if (strcmp (cmdStr, "INT") == 0)
    {
        code[pos++] = INT;
    }
    else if (strcmp (cmdStr, "DISP") == 0)
    {
        code[pos++] = DISP;
    }
    else if (strcmp (cmdStr, "MOD") == 0)
    {
        code[pos++] = MOD;
    }
    else if (strcmp (cmdStr, "POP") == 0)
    {
        code[pos++] = POP;
    }
    else if (strcmp (cmdStr, "JMP") == 0)
    {  // TODO: AssembleJump
        code[pos++] = JMP;

        char argument[10] = {};

        fscanf(file_asm, "%s", argument);

        if (argument[0] == ':')
        {
            int pos_labels = take_label_num(argument);
            printf("\n\nlabel: %d \n\n", pos_labels);
            int address = get_addres_label(pos_labels, labels);
            if (address == -1)
            {
                printf("ERROR: Label %s not found!\n", argument);
                free(code);
                fclose(file_asm);
            }
            code[pos++] = address;                                                                                                                          printf ("vvpdrs\n");

            printf("JMP to %s (address %d)\n", argument, address);
        }
        else
        {
            int arg = 0;

            fscanf(file_asm, "%d", &arg);

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

        fscanf(file_asm, "%s", argument);

        if (argument[0] == ':')
        {
            int pos_labels = take_label_num(argument);
            int address = get_addres_label(pos_labels, labels);

            if (address == -1)
            {
                printf("ERROR: Label %s not found!\n", argument);
                free(code);
                fclose(file_asm);
            }
            code[pos++] = address;

            printf("JMP to %s (address %d)\n", argument, address);
        }
        else
        {
            int arg = 0;

            fscanf(file_asm, "%d", &arg);

            code[pos++] = arg;
        }
    }
    else if (strcmp (cmdStr, "JBE") == 0)
    {
        code[pos++] = JBE;

        char argument[10] = {};

        fscanf(file_asm, "%s", argument);

        if (argument[0] == ':')
        {
            int pos_labels = take_label_num(argument);
            int address = get_addres_label(pos_labels, labels);
            if (address == -1)
            {
                printf("ERROR: Label %s not found!\n", argument);
                free(code);
                fclose(file_asm);
            }
            code[pos++] = address;

            printf("JMP to %s (address %d)\n", argument, address);
        }
        else
        {
            int arg = 0;

            fscanf(file_asm, "%d", &arg);

            code[pos++] = arg;
        }
    }
    else if (strcmp (cmdStr, "JA") == 0)
    {
        code[pos++] = JA;

        char argument[10] = {};

        fscanf(file_asm, "%s", argument);

        if (argument[0] == ':')
        {
            int pos_labels = take_label_num(argument);
            int address = get_addres_label(pos_labels, labels);
            if (address == -1)
            {
                printf("ERROR: Label %s not found!\n", argument);
                free(code);
                fclose(file_asm);
            }
            code[pos++] = address;

            printf("JMP to %s (address %d)\n", argument, address);
        }
        else
        {
            int arg = 0;

            fscanf(file_asm, "%d", &arg);

            code[pos++] = arg;
        }
    }
    else if (strcmp (cmdStr, "JAE") == 0)
    {
        code[pos++] = JAE;

        char argument[10] = {};

        fscanf(file_asm, "%s", argument);

        if (argument[0] == ':')
        {
            int pos_labels = take_label_num(argument);
            int address = get_addres_label(pos_labels, labels);
            if (address == -1)
            {
                printf("ERROR: Label %s not found!\n", argument);
                free(code);
                fclose(file_asm);
            }
            code[pos++] = address;

            printf("JMP to %s (address %d)\n", argument, address);
        }
        else
        {
            int arg = 0;

            fscanf(file_asm, "%d", &arg);

            code[pos++] = arg;
        }
    }
    else if (strcmp (cmdStr, "JE") == 0)
    {
        code[pos++] = JE;

        char argument[10] = {};

        fscanf(file_asm, "%s", argument);

        if (argument[0] == ':')
        {
            int label_num = take_label_num(argument);
            int address = get_addres_label(label_num, labels);
            if (address == -1)
            {
                printf("ERROR: Label %s not found!\n", argument);
                free(code);
                fclose(file_asm);
            }
            code[pos++] = address;

            printf("JMP to %s (address %d)\n", argument, address);
        }
        else
        {
            int arg = 0;

            fscanf(file_asm, "%d", &arg);

            code[pos++] = arg;
        }
    }
    else if (strcmp (cmdStr, "JNE") == 0)
    {
        code[pos++] = JNE;

        char argument[10] = {};

        fscanf(file_asm, "%s", argument);

        if (argument[0] == ':')
        {
            int pos_labels = take_label_num(argument);
            int address = get_addres_label(pos_labels, labels);
            if (address == -1)
            {
                printf("ERROR: Label %s not found!\n", argument);
                free(code);
                fclose(file_asm);
            }
            code[pos++] = address;

            printf("JMP to %s (address: %d)\n", argument, address);
        }
        else
        {
            int arg = 0;

            fscanf(file_asm, "%d", &arg);

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

        char argument[10] = {};

        fscanf(file_asm, "%s", argument);

        if (argument[0] == ':')
        {
            int pos_labels = take_label_num(argument);
            printf("\n\nlabel: %d \n\n", pos_labels);
            int address = get_addres_label(pos_labels, labels);
            if (address == -1)
            {
                printf("ERROR: Label %s not found!\n", argument);
                free(code);
                fclose(file_asm);
            }
            code[pos++] = address;                                                                                                                          printf ("vvpdrs\n");

            printf("JMP to %s (address %d)\n", argument, address);
        }
        else
        {
            int arg = 0;

            fscanf(file_asm, "%d", &arg);

            code[pos++] = arg;
        }
    }
    else if (strcmp (cmdStr, "PUSHM") == 0)
    {
        code[pos++] = PUSHM;

        char argument[10] = {};

        fscanf(file_asm, "%s", argument);

        if (strlen(argument) >= 4 &&
        argument[0] == '[' && argument[strlen(argument)-1] == ']')
        {
            // Извлекаем имя регистра между скобками
            char reg_name[10] = "";
            strncpy(reg_name, argument + 1, strlen(argument) - 2);
            reg_name[strlen(argument) - 2] = '\0';

            printf("DEBUG: PUSHM register name: '%s'\n", reg_name);

            if (strcmp(reg_name, "Ax") == 0) code[pos++] = Ax;
            else if (strcmp(reg_name, "Bx") == 0) code[pos++] = Bx;
            else if (strcmp(reg_name, "Cx") == 0) code[pos++] = Cx;
            else if (strcmp(reg_name, "Dx") == 0) code[pos++] = Dx;
            else {
                printf("ERROR: Unknown register in memory access: %s\n", reg_name);
                free(code);
                fclose(file_asm);
            }
        }
        else
        {
            printf("ERROR: Invalid memory access syntax: %s\n", argument);
            free(code);
            fclose(file_asm);
        }
    }
    else if (strcmp (cmdStr, "POPM") == 0)
    {
        code[pos++] = POPM;

        char argument[10] = {};

        fscanf(file_asm, "%s", argument);

        if (strlen(argument) >= 4 &&
        argument[0] == '[' && argument[strlen(argument)-1] == ']')
        {
            char reg_name[10] = "";
            strncpy(reg_name, argument + 1, strlen(argument) - 2);
            reg_name[strlen(argument) - 2] = '\0';

            printf("DEBUG: PUSHM register name: '%s'\n", reg_name);

            if (strcmp(reg_name, "Ax") == 0) code[pos++] = Ax;
            else if (strcmp(reg_name, "Bx") == 0) code[pos++] = Bx;
            else if (strcmp(reg_name, "Cx") == 0) code[pos++] = Cx;
            else if (strcmp(reg_name, "Dx") == 0) code[pos++] = Dx;
            else {
                printf("ERROR: Unknown register in memory access: %s\n", reg_name);
                free(code);
                fclose(file_asm);
            }
        }
        else
        {
            printf("ERROR: Invalid memory access syntax: %s\n", argument);
            free(code);
            fclose(file_asm);
        }
    }
    else
        SyntaxERROR(pos, cmdStr);

    *pos_ptr = pos;

    return code;
}*/


int* Command_with_value(int* code, StructCmd* all_Cmd, int* pos_ptr, FILE* file_asm, size_t i)
{
    assert(pos_ptr);
    assert(code);

    int pos = *pos_ptr;

    code[pos++] = all_Cmd[i].Cmd;

    int arg = 0;

    fscanf(file_asm, "%d", &arg);

    code[pos++] = arg;

    *pos_ptr = pos;
    return code;
}

int* Command_for_Jump (int* code, StructCmd* all_Cmd, int* pos_ptr, FILE* file_asm, size_t i, Labels* labels)
{
    assert(pos_ptr);
    assert(code);

    int pos = *pos_ptr;

    code[pos++] = all_Cmd[i].Cmd;

    char argument[10] = {};

    fscanf(file_asm, "%s", argument);

    printf("DEBUG: Processing jump command %s with argument %s at position %d\n", all_Cmd[i].name, argument, pos-1);

    if (argument[0] == ':')
    {
        int label_num = take_label_num(argument);
        int address = get_addres_label(label_num, labels);

        printf("DEBUG: Label %s -> num %d -> address %d\n", argument, label_num, address);

        if (address == -1)
        {
            printf("ERROR: Label %s not found!\n", argument);
            free(code);
            fclose(file_asm);
        }
        code[pos++] = address;
         printf("DEBUG: Written address %d at position %d\n", address, pos-1);

        printf("JMP to %s (address %d)\n", argument, address);
    }
    else
    {
        int arg = 0;

        fscanf(file_asm, "%d", &arg);

        code[pos++] = arg;
        printf("DEBUG: Written immediate value %d at position %d\n", arg, pos-1);
    }
    *pos_ptr = pos;

    return code;
}

int* Command_for_mem (int* code, StructCmd* all_Cmd, int* pos_ptr, FILE* file_asm, size_t i)
{
    assert(pos_ptr);
    assert(code);

    int pos = *pos_ptr;

    code[pos++] = all_Cmd[i].Cmd;

    char argument[10] = {};

    fscanf(file_asm, "%s", argument);

    if (strlen(argument) >= 4 && argument[0] == '[' && argument[strlen(argument)-1] == ']')
    {
        char reg_name[10] = "";
        strncpy(reg_name, argument + 1, strlen(argument) - 2);
        reg_name[strlen(argument) - 2] = '\0';

        printf("DEBUG: register name: '%s'\n", reg_name);

        if (strcmp(reg_name, "Ax") == 0) code[pos++] = Ax;
        else if (strcmp(reg_name, "Bx") == 0) code[pos++] = Bx;
        else if (strcmp(reg_name, "Cx") == 0) code[pos++] = Cx;
        else if (strcmp(reg_name, "Dx") == 0) code[pos++] = Dx;
        else
        {
            printf("ERROR: Unknown register in memory access: %s\n", reg_name);
            free(code);
            fclose(file_asm);
        }
    }
    else
    {
        printf("ERROR: Invalid memory access syntax: %s\n", argument);
        free(code);
        fclose(file_asm);
    }

    *pos_ptr = pos;
    return code;
}

int* Command_for_reg (int* code, StructCmd* all_Cmd, int* pos_ptr, FILE* file_asm, size_t i)
{
    assert(pos_ptr);
    assert(code);

    int pos = *pos_ptr;

    code[pos++] = all_Cmd[i].Cmd;

    char argument[10] = {};

    fscanf(file_asm, "%s", argument);

    if (strcmp(argument, "Ax") == 0) code[pos++] = Ax;
    else if (strcmp(argument, "Bx") == 0) code[pos++] = Bx;
    else if (strcmp(argument, "Cx") == 0) code[pos++] = Cx;
    else if (strcmp(argument, "Dx") == 0) code[pos++] = Dx;
    else
    {
        fprintf(stderr, "ERORR REGISTER !!!");
    }

    *pos_ptr = pos;
    return code;
}

int* Command_for_call(int* code, StructCmd* all_Cmd, int* pos_ptr, FILE* file_asm, size_t i, Labels* labels)
{
    assert(pos_ptr);
    assert(code);

    int pos = *pos_ptr;

    code[pos++] = all_Cmd[i].Cmd;

    char argument[10] = {};

    fscanf(file_asm, "%s", argument);

    if (argument[0] == ':')
    {
        int pos_labels = take_label_num(argument);
        printf("\n\nlabel: %d \n\n", pos_labels);
        int address = get_addres_label(pos_labels, labels);
        if (address == -1)
        {
            printf("ERROR: Label %s not found!\n", argument);
            free(code);
            fclose(file_asm);
        }
        code[pos++] = address;                                                                                                                          printf ("vvpdrs\n");

        printf("JMP to %s (address %d)\n", argument, address);
    }
    else
    {
        int arg = 0;

        fscanf(file_asm, "%d", &arg);

        code[pos++] = arg;
    }

    *pos_ptr = pos;
    return code;
}
