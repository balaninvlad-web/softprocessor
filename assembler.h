#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "enum.h"
#include "stack_functions.h"

#define MAX_LABELS 10

struct Labels
{
    int labels[MAX_LABELS];
};

struct StructCmd
{
    const char* name;
    int Cmd;
    ArgType arg;
    //TODO указатель на функцию сюда добавить
};
const int MAX_COUNT_COMMAND = 27;

extern StructCmd command_table[MAX_COUNT_COMMAND];

/*struct CallStack
{
    my_stack_t* stack_call;
};*/


int is_label (const char* str);
int take_label_num (const char* label_str);
int get_addres_label (int pos, Labels* labels_table);
void make_labels( Labels* labels_table);
void add_labels(int pos, int address,  Labels* labels_table);
int SyntaxERROR(int pos, char* smdStr);
void first_go (FILE* test,  Labels* labels_table);
int ExtendCode(int* capacity, int** code);
//int* AssembleCommand(char* cmdStr, FILE* test, int* code, Labels* labels, int* pos);
int PrintToBinaryFile(int* code, int pos, const char* file_name);

int* find_command(char* cmdStr, FILE* file_asm, int* code, Labels* labels, int* pos_ptr, StructCmd* all_Cmd);int* Command_with_value(int* code, StructCmd* all_Cmd, int* pos, FILE* file_asm, size_t i);
int* Command_for_Jump(int* code, StructCmd* all_Cmd, int* pos_ptr, FILE* file_asm, size_t i, Labels* labels);
int* Command_for_reg(int* code, StructCmd* all_Cmd, int* pos_ptr, FILE* file_asm, size_t i);
int* Command_for_mem(int* code, StructCmd* all_Cmd, int* pos_ptr, FILE* file_asm, size_t i);
int* Command_for_call(int* code, StructCmd* all_Cmd, int* pos_ptr, FILE* file_asm, size_t i, Labels* labels);

#endif
