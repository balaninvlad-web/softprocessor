INN
POPM [Ax]
PUSH 1
POPM [Bx]

:1
PUSHM [Ax]
PUSH 1
JB :2
JE :2

PUSHM [Bx]
PUSHM [Ax]
MUL
POPM [Bx]

PUSHM [Ax]
PUSH 1
SUB
POPM [Ax]

JMP :1

:2
PUSHM [Bx]
OUT
HTL
