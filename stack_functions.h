#ifndef STACK_FUNCTIONS_H
#define STACK_FUNCTIONS_H
#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "enum.h"


#define __vovin
#define __seregin
#define __dedov

//#define $$$$(...)  __VA_ARGS__

/*#define get_type_name(x) _Generic((x), \
    int: "int", \
    float: "float", \
    double: "double", \
    char: "char", \
    char*: "string", \
    default: "unknown" \
)*/

typedef int StackErr_t;

typedef struct
{
    int* data;
    size_t capacity;
    size_t sizestk;

} my_stack_t;

enum nameOFerrWORK
{
    NOERORR,
    ERORRDATANULL,
    ERORRPTR,
    ERORRPOLNIISTECK,
    ERORRPOP,
};

enum nameOFerr
{
    ERORRPOINTER        = 1 << 0,
    ERORRBUFER          = 1 << 1,
    ERORRSCHETCHIK      = 1 << 2,
    ERORRSIZE           = 1 << 3,
    ERORRSIZE_T         = 1 << 4,
    MAXXSIZEERORR       = 1 << 5,
    MAXXCAPACITYERORR   = 1 << 6,
    FIRSTERORRCANAREICA = 1 << 7,
    LASTERORRCANAREICA  = 1 << 8
};

extern const int START_OF_DATA;
extern const int CANAREICA1;
extern const int CANAREICA2;
extern const int SWAG;
extern const size_t MAXVALUE;

StackErr_t StackCtor (my_stack_t* stk1, size_t capacity);
StackErr_t StackPush (my_stack_t* stk1, int value);
StackErr_t StackPop (my_stack_t* stk1, int* value);
StackErr_t StackDtor (my_stack_t* stk1);
StackErr_t verificator (my_stack_t* stk1, const char* file, const char* func, int line);
StackErr_t StackDump (my_stack_t* stk1, int i, const char* file, const char* func, int line);
void change_of_type(int i);

#endif
