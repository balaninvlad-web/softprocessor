#include "stack_functions.h"
#include "processor.h"

//#define RAM_SIZE 100

int Processor_calculate (my_pro_t* str_pro,const char* file_name)
{
    fprintf(stderr, "Loading file pro: %s\n", file_name);

    const char* test = file_name;

    fprintf(stderr, "\n\n%s\n\n", test);

    assert(str_pro->buffer != NULL);

    int* buffer = str_pro->buffer;

    //buffer = read_from_file(str_pro, test, &size_of_buffer);

    int command = 0;
    str_pro->ip = 0;

    assert(&str_pro->stk_for_calculate.data != NULL);

    while (1)
    {
        command = buffer[str_pro->ip];

        #ifdef DEBUG
            $_(str_pro->ip);
            $(command);
        #endif
        str_pro->ip++;

        if(command == HTL)
        {
            printf("PROGRAM finish with HTL\n");
            break;
        }

        switch (command)
        {
            case PUSH:
            case INN:
            {
                Push_printf (0, str_pro, command, buffer);
                break;
            }
            case ADD:
            case SUB:
            case MUL:
            case DIV:
            case MOD:
            {
                Calculate_func_first (command, str_pro);
                break;
            }
            case DISP:
            case POP:
            case OUTP:
            case POW: // TODO: vozvedenie v stepen'
            case SQRT:
            {
                One_value_calc (command, str_pro);
                break;
            }
            case PUSHREG:
            case POPREG:
            {
                Do_for_reg (command, str_pro, buffer);
                break;
            }
            case JMP:
            {
                Do_Jump_if_true(str_pro, buffer, command);
                break;
            }
            case JB:
            case JAE:
            case JA:
            case JBE:
            case JE:
            case JNE:
            {
                Do_if_Jmp (command, str_pro, buffer);
                break;
            }
            case CALL:
            {
                Do_if_Call(str_pro, buffer, command);
                break;

            }
            case RET:
            {
                int ret_pos = 0;

                StackPop(&str_pro->stk_call, &ret_pos);

                str_pro->ip = ret_pos;
                break;
            }
            case PUSHM:
            case POPM:
            {
                Mem_func (command, str_pro, buffer);
                break;
            }
            default:
                fprintf(stderr, "Unknown command: %d\n", command);
                return false;
        }
    }
    ProcessorDtor (str_pro);
    return 0;
}

void Calculate_func_first (int command, my_pro_t* str_pro)
{
    int x = 0;
    int y = 0;

    StackPop(&str_pro->stk_for_calculate, &x);
    StackPop(&str_pro->stk_for_calculate, &y);

    #ifdef DEBUG
        fprintf(stderr, "%d values is:\n%d\n%d\n", x, y, command);
    #endif

    int received_value = 0;

    switch(command)
    {
        case ADD:
            received_value = x + y;
            break;
        case SUB:
            received_value = y - x;
            break;
        case MUL:
            received_value = x * y;
            break;
        case DIV:
            if (x == 0)
                printf("ERORR: X value is ZERO\n");
            received_value = y / x;
            break;
        case MOD:
            received_value = y % x;
            break;
        default:
            break;
    }

    Push_printf (received_value, str_pro, 777, NULL);
}

void Push_printf(int received_value, my_pro_t* str_pro, int command, int* buffer)
{
    switch (command)
    {
        case PUSH:
        {
            int value_cal = buffer[str_pro->ip];

            DUMP_PRO(str_pro, command);

            str_pro->ip++;

            StackPush(&str_pro->stk_for_calculate, value_cal);

            DUMP_PRO(str_pro, command);
            break;
        }
        case INN:
        {
            int value_cal = 0;

            fprintf(stderr,"INN VALUE: ");

            scanf("%d", &value_cal);

            #ifdef DEBUG
                fprintf(stderr, "PUSHED VALUE: %d\n",value_cal);
            #endif

            StackPush(&str_pro->stk_for_calculate, value_cal);

            DUMP_PRO(str_pro, command);
            break;
        }
        case 777:
        {
            StackPush(&str_pro->stk_for_calculate, received_value);

            #ifdef DEBUG
                fprintf(stderr,"\n\npushed value: %d\n\n", received_value);
            #endif
            break;
        }
        default:
            fprintf(stderr, "Unknown command: %d\n", command);
            break;
    }

    DUMP_PRO(str_pro, command);
}

bool IfCondJump(int command, int y, int x)
{
    switch (command)
    {
        case JB:
            return y < x;
        case JBE:
            return y <= x;
        case JA:
            return y > x;
        case JAE:
            return y >= x;
        case JE:
            return y == x;
        case JNE:
            return y != x;
        default:
            return false;
    }
}
/*int* getRegisterAddress(my_pro_t* str_pro,int curr_register_num)
{
    switch (curr_register_num)
    {
        case Ax:
            #ifdef DEBUG
                printf("Ax");
            #endif
            return &str_pro->registers[POS_Ax]; // TODO убрать этот свитч вообще, переделать енам регистров
        case Bx:
            #ifdef DEBUG
                printf("Bx");
            #endif
            return &str_pro->registers[POS_Bx];
        case Cx:
            #ifdef DEBUG
                printf("Cx");
            #endif
            return &str_pro->registers[POS_Cx];
        case Dx:
            #ifdef DEBUG
                printf("Dx");
            #endif
            return &str_pro->registers[POS_Dx];
        default:
            return NULL;
    }
}*/
const char* Get_Name_registr (int curr_register_num)
{
    switch (curr_register_num)
    {
        case Ax: return "Ax";
        case Bx: return "Bx";
        case Cx: return "Cx";
        case Dx: return "Dx";
        default: return "unknown";
    }
}

void Do_Jump_if_true(my_pro_t* str_pro, int* buffer, int command)
{
    int value_cal = buffer[str_pro->ip];

    #ifdef DEBUG
        fprintf(stderr, "STR JUMP: %d\n",value_cal);
    #endif

    str_pro->ip = value_cal + 1;

    DUMP_PRO(str_pro, command);
}

void One_value_calc (int command, my_pro_t* str_pro)
{

    assert(str_pro);

    int x = 0;

    StackPop(&str_pro->stk_for_calculate, &x);

    #ifdef DEBUG
        fprintf(stderr,"%d\n", x);
    #endif

    int received_value = 0;

    switch(command)
    {
        case POP:
            fprintf(stderr, "POPED VALUE: %d", x);
            break;
        case OUTP:
            printf("\nOUT VALUE: %d\n", x);
            break;
        case POW:
            received_value = x * x;

            #ifdef DEBUG
                fprintf(stderr, "\n\nPOW value: %d\n", received_value);
            #endif

            StackPush(&str_pro->stk_for_calculate, received_value);

            DUMP_PRO(str_pro, command);
            break;
        case SQRT:
            received_value = (int) sqrt((double) x);

            StackPush(&str_pro->stk_for_calculate, received_value);

            DUMP_PRO(str_pro, command);
            break;
        case DISP:
            printf("%c", x);
            break;
        default:
            fprintf(stderr, "Unknown command: %d\n", command);
            break;
    }
}

void Mem_func (int command, my_pro_t* str_pro, int* buffer)
{
    int curr_register_num = buffer[str_pro->ip];

    str_pro->ip++;

    int* mem_address = &str_pro->registers[curr_register_num];

    int value = 0;

    if (*mem_address >= 0 && *mem_address <= 100)
    {
        switch(command)
        {
            case POPM:
                StackPop(&str_pro->stk_for_calculate, &value);

                str_pro->Ram_Mem [*mem_address] = value;

                printf("PUSHM [%s]: value %d from address %d\n", Get_Name_registr(curr_register_num), value, *mem_address);

                break;
            case PUSHM:
                value = str_pro->Ram_Mem [*mem_address];

                StackPush(&str_pro->stk_for_calculate, value);

                printf("PUSHM [%s]: value %d from address %d\n", Get_Name_registr(curr_register_num), value, *mem_address);

                break;
            default:
                fprintf(stderr, "Unknown command: %d\n", command);
                break;
        }
    }
    else
        printf("ERORR, BAD mem_address: %d\n", *mem_address);

    DUMP_PRO(str_pro, command);
}

void Do_if_Call (my_pro_t* str_pro, int* buffer, int command)
{
    StackPush(&str_pro->stk_call, str_pro->ip);

    int value_cal = buffer[str_pro->ip];

    #ifdef DEBUG
        fprintf(stderr, "STR CALL: %d\n",value_cal);
    #endif

    str_pro->ip++;

    str_pro->ip = value_cal;

    DUMP_PRO(str_pro, command);
}

void Do_if_Jmp (int command, my_pro_t* str_pro, int* buffer)
{
    int x = 0;
    int y = 0;

    StackPop(&str_pro->stk_for_calculate, &x);
    StackPop(&str_pro->stk_for_calculate, &y);
    $_(x);
    $_(y);

    if (IfCondJump(command, x, y))
    {
        Do_Jump_if_true(str_pro, buffer, command);
    }
    else
        str_pro->ip++;
}

void Do_for_reg (int command, my_pro_t* str_pro, int* buffer)
{
    int curr_register_num = buffer[str_pro->ip];

    int* reg_addr = &str_pro->registers[curr_register_num];

    assert(reg_addr);
    assert(&str_pro->stk_for_calculate);

    switch (command)
    {
        case PUSHREG:
        {
            #ifdef DEBUG
                printf("PUSHREG: ");
            #endif

            StackPush(&str_pro->stk_for_calculate, *reg_addr);

            printf(": %d\n", *reg_addr);
            break;
        }
        case POPREG:
        {
            #ifdef DEBUG
                printf("POPREG");
            #endif

            StackPop(&str_pro->stk_for_calculate, reg_addr);

            #ifdef DEBUG
                printf(": %d\n", *reg_addr);
            #endif
            break;
        }
        default:
            fprintf(stderr, "Unknown command: %d\n", command);
            break;
    }
    str_pro->ip++;

    DUMP_PRO(str_pro, command);
}
