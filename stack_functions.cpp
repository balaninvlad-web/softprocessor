#include "stack_functions.h"

const int START_OF_DATA = 1;
const int CANAREICA1 = 217217217;
const int CANAREICA2 = 333333333;
const int SWAG = 0xC0C1DED;
const size_t MAXVALUE = ((size_t)(-1))/2;

#define POIZON SWAG

//define DEBUG

#define REALISE

StackErr_t StackCtor (my_stack_t* stk1, size_t capacity)
{
    assert(stk1);

    stk1->data = (int*) calloc (capacity + 2, sizeof(int));

    if (stk1->data == NULL)
    {
        return ERORRDATANULL;
    }

    stk1->data[0] = CANAREICA1;
    stk1->data[capacity + 1] = CANAREICA2;

    stk1->capacity = capacity;
    stk1->sizestk = START_OF_DATA;

    #ifdef DEBUG
        verificator (stk1, __FILE__, __func__ ,__LINE__);
    #endif
    return NOERORR;
}

StackErr_t StackPush (my_stack_t* stk1, int value)
{
    #ifdef DEBUG
        verificator (stk1, __FILE__, __func__ ,__LINE__);
    #endif

    if (stk1 -> sizestk < stk1 -> capacity)
    {
        stk1 -> data[stk1 -> sizestk] = value;

        stk1 -> sizestk++;

        if (stk1 -> sizestk >= stk1 -> capacity)
        {
            size_t new_capacity = (stk1 -> capacity == 0) ? START_OF_DATA : stk1 -> capacity * 2;

            int* new_data = (int*)realloc(stk1 -> data, new_capacity * sizeof(int));

            if (new_data == nullptr)
            {
                return ERORRPTR;
            }

            stk1->data = new_data;
            stk1->capacity = new_capacity;
            stk1->data[stk1->capacity + 1] = CANAREICA2;
        }

        return NOERORR;
    }

    #ifdef DEBUG
        verificator (stk1, __FILE__, __func__ ,__LINE__);
    #endif

    return ERORRPOLNIISTECK; //polnii steck
}

StackErr_t StackPop (my_stack_t* stk1, int* value)
{
    #ifdef DEBUG
        verificator (stk1, __FILE__, __func__ ,__LINE__);
    #endif

    if (stk1 -> sizestk > START_OF_DATA) //TODO kjbkhb START_OF_DATA
    {
         stk1 -> sizestk--;

        *value = stk1 -> data[stk1 ->sizestk];
        stk1 -> data[stk1 ->sizestk] = POIZON;



        #ifdef DEBUG
            fprintf(stderr,"POPED value:%d\n", *value);

            verificator (stk1, __FILE__, __func__ ,__LINE__);
        #endif

        return *value;
    }

    #ifdef DEBUG
        verificator (stk1, __FILE__, __func__ ,__LINE__);
    #endif

    return ERORRPOP;
}

StackErr_t StackDtor (my_stack_t* stk1)
{
    //stk1 -> sizestk = 1000000;

    #ifdef DEBUG
        verificator (stk1, __FILE__, __func__ ,__LINE__);
    #endif

    free(stk1 -> data);

    stk1 -> data = NULL;
    stk1 -> capacity = 0;
    stk1 -> sizestk = 0;

    return NOERORR;
}

StackErr_t verificator (my_stack_t* stk1, const char* file, const char* func, int line)
{
    int i = 0;
    //1 проверка
    if (stk1 == NULL)
    {
        i |= ERORRPOINTER;
    }
    //2 проверка
    if (stk1 -> data == NULL)
    {
        i |= ERORRBUFER;
    }
    /*//3 проверка
    if (stk1 -> sizestk < 0)
    {
        i |= ERORRSCHETCHIK;
    }
    //4 проверка
    if (stk1 -> capacity < 0) //TODO: razobratsa kak moschno size_t cravnit' c 0
    {
        i |= ERORRSIZE;
    }*/
    //5 проверка
    if (stk1 -> capacity == 0)
    {
        i |= ERORRSIZE_T;
    }
    //6 проверка
    if (stk1 -> sizestk >= MAXVALUE)// TODO vivodit ochibky tipo
    {
        i |= MAXXSIZEERORR;
    }
    //7 проверка
    if (stk1 -> capacity >= MAXVALUE)
    {
        i |= MAXXCAPACITYERORR;
    }
    //8 проверка
    if (stk1 -> data[0] != CANAREICA1) //TODO const for canareica
    {
        i |= FIRSTERORRCANAREICA;
    }
    //9 проверка
    if (stk1 -> data[stk1 -> capacity + 1] != CANAREICA2) //TODO pochemy ne robit
    {
        i |= LASTERORRCANAREICA;
    }

    if (i > 0)
        StackDump(stk1, i, file, func, line);

    return i;
}

StackErr_t StackDump (my_stack_t* stk1, int i, const char* file, const char* func, int line)
{
    fprintf(stderr, "-------------StackDumb----------------\n");

    fprintf(stderr, "Stack <int> in file: %s, function: %s, in line: %d", file, func, line);
    fprintf(stderr, "[%p]\n", (void*)stk1);

    //print_name_of_erorr();

    fprintf(stderr, "kod oshibki: %d\n", i);
    fprintf(stderr, "dvoichnii kod ochibki: ");

    change_of_type(i);

    if (stk1 != NULL)
    {
        fprintf(stderr, "\n size: %lu\n", (unsigned long)stk1 -> sizestk); // Исправил формат
        fprintf(stderr, "capacity: %lu\n", (unsigned long)stk1->capacity);

        if (stk1->data != NULL)
        {
            fprintf(stderr, "data pointer: %p\n", (void*)stk1->data);

            fprintf(stderr, "data values: \n");
            for (size_t j = 0; j < stk1->sizestk; j++)
            {
                fprintf(stderr, "*[%d] = %d \n", j, stk1->data[j]);
            }

            for (size_t __vovin penis = stk1->sizestk; penis < stk1 -> capacity + 2; penis++)
            {
                fprintf(stderr, " [%d] = %06X (POISON)\n", penis, stk1 -> data[penis]);
            }
            fprintf(stderr, "\n");
        }
        else
        {
            fprintf(stderr, "data: NULL\n");
        }
    }
    else
    {
        fprintf(stderr, "stack pointer: NULL\n");
    }
    fprintf(stderr, "-------------end of dumb-------------\n");
    return 0;
}

void change_of_type(int i)
{
    for (int j = 31; j >= 0; j--)
        fprintf(stderr, "%d", (i >> j) & 1);
}
