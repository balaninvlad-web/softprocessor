#ifndef ENUM_H
#define ENUM_H

#include <TXLib.h>

typedef enum
{
    no_arg = 0,
    value = 1,
    JMP_arg = 2,
    reg_val = 3,
    call_arg = 4,
    mem_arg = 5
} ArgType;

enum nameOFcommand
{
    PUSH    = 1,
    ADD     = 2,
    SUB     = 3,
    DIV     = 4,
    OUTP    = 5,
    SQRT    = 6,
    MUL     = 7,
    POW     = 8,
    JMP     = 9,
    POP     = 10,
    HTL     = 11,
    PUSHREG = 33,
    POPREG  = 52,
    JB      = 12,
    JBE     = 13,
    JA      = 14,
    JAE     = 15,
    JE      = 16,
    JNE     = 17,
    INN     = 18,
    RET     = 19,
    CALL    = 20,
    PUSHM   = 21,
    POPM    = 22,
    MOD     = 23,
    DISP    = 25,
};

enum nameOFregistr
{
    Ax = 1,
    Bx = 2,
    Cx = 3,
    Dx = 4,
};

#endif
