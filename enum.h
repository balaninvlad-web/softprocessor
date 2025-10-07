#ifndef ENUM_H
#define ENUM_H

#include <TXLib.h>

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
};

enum nameOFregistr
{
    Ax = 111,
    Bx = 222,
    Cx = 333,
    Dx = 444,
};

#endif
