#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>

#include "stack_functions.h"
#include "enum.h"

#define POS_Ax 1
#define POS_Bx 2
#define POS_Cx 3
#define POS_Dx 4

typedef struct
{
    my_stack_t stk1;
    int* buffer;
    size_t ip;
    int registers[4];
    my_stack_t stk_call;
    int Ram_Mem [100] ={};

} my_pro_t;

int Calculator(my_pro_t* stk1, char* file_name);
int* read_from_file(my_pro_t* str_pro, const char* TEST, size_t* size_of_buffer);
int ProcessorCtor(my_pro_t* str_pro);
void ProcessorDtor(my_pro_t* str_pro);
int DumpPro (my_pro_t* str_pro,int i, const char* file, const char* func, int line, int command);
void Do_Jump_if_true(my_pro_t* str_pro, int* buffer, int command);
int* getRegisterAddress(my_pro_t* str_pro,int curr_register_num);
bool IfCondJump(int command, int x, int y);
void Push_printf (int s, my_pro_t* str_pro, int command);
int Calculate_func_first (int command, my_pro_t* str_pro);
const char* Get_Name_registr (int curr_register_num);
void One_value_calc (int command, my_pro_t* str_pro);

#endif
