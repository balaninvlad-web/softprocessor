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

/*struct CallStack
{
    my_stack_t* stack_call;
};*/


int num_of_label (const char* str);
int take_label_num (const char* label_str);
int get_addres_label (int pos, Labels* labels_table);
void make_labels( Labels* labels_table);
void addlabels(int pos, int address,  Labels* labels_table);
int SyntaxERROR(int pos, char* smdStr);
void first_go (FILE* test,  Labels* labels_table);



#endif
