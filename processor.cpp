#include "stack_functions.h"
#include "processor.h"

//#define RAM_SIZE 100

/*int* read_from_file(const char* TEST, size_t* size_of_buffer)
{
    FILE* input_file = fopen(TEST, "rb");

    assert(input_file != NULL);

    struct stat statbuf ={};

    stat (TEST, &statbuf);

    *size_of_buffer = statbuf.st_size;

    assert(*size_of_buffer != 0);

    $(*size_of_buffer);

    int* buffer = (int*)calloc(*size_of_buffer + 1, sizeof(char));

    assert(buffer != NULL);

    buffer[*size_of_buffer] = '\0';

    fread(buffer, sizeof(int), *size_of_buffer, input_file);

    assert(buffer != NULL);

    return buffer;
}*/

int Calculator (my_pro_t* str_pro, char* file_name)
{
    //char bin_filename[100] ="";
    //printf("Введите имя бинарног файла :");
    //scanf("%99s", bin_filename);

    fprintf(stderr, "Loading file: %s\n", file_name);

    const char* TEST = file_name;

    fprintf(stderr, "\n\n%s\n\n", TEST);

    int* buffer = 0;
    size_t size_of_buffer = 0;

    buffer = read_from_file(str_pro, TEST, &size_of_buffer);

    int value_cal = 0;
    int command = 0;
    str_pro->ip = 0;


    assert(&str_pro->stk1.data != NULL);

    while (command != HTL)
    {
        command = buffer[str_pro->ip];

        #ifdef DEBUG
            $_(str_pro->ip);
            $(command);
        #endif

        str_pro->ip++;

        switch (command)
        {
            case PUSH:
            {
                value_cal = buffer[str_pro->ip];

                #ifdef DEBUG
                    fprintf(stderr, "PUSHED VALUE: %d\n",value_cal);
                #endif

                str_pro->ip++;

                StackPush(&str_pro->stk1, value_cal);
                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__, command);
                #endif
                break;
            }

            case INN:
            {
                value_cal = 0;

                fprintf(stderr,"INN VALUE: ");

                scanf("%d", &value_cal);

                #ifdef DEBUG
                    fprintf(stderr, "PUSHED VALUE: %d\n",value_cal);
                #endif

                StackPush(&str_pro->stk1, value_cal);
                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__, command);
                #endif
                break;
            }

            case ADD:
            case SUB:
            case MUL:
            case DIV:
            case MOD:
            {
                int s = Calculate_func_first (command, str_pro);

                Push_printf (s, str_pro, command);
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
            {
                command = buffer[str_pro->ip];

                int curr_register_num = buffer[str_pro->ip];

                #ifdef DEBUG
                    printf("PUSHREG ");
                #endif

                int* reg_addr = getRegisterAddress(str_pro, curr_register_num);

                assert(reg_addr);
                assert(&str_pro->stk1);

                StackPush(&str_pro->stk1, *reg_addr);

                printf(": %d\n", *reg_addr);

                str_pro->ip++;

                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__, command);
                #endif

                break;
            }
            case POPREG:
            {
                command = buffer[str_pro->ip];

                int curr_register_num = buffer[str_pro->ip];

                #ifdef DEBUG
                    printf("POPREG ");
                #endif

                int* reg_addr = getRegisterAddress(str_pro, curr_register_num);

                #ifdef DEBUG
                    printf(":\n");
                #endif

                StackPop(&str_pro->stk1, reg_addr);

                str_pro->ip++;

                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__, command);
                #endif
                break;
            }
            case JMP:
            {
                value_cal = buffer[str_pro->ip];
                #ifdef DEBUG
                    fprintf(stderr, "STR JUMP: %d\n",value_cal);
                #endif
                str_pro->ip = value_cal + 1;

                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__, command);
                #endif
                break;
            }

            case JB:
            case JAE:
            case JA:
            case JBE:
            case JE:
            case JNE:
            {

                int x = 0;
                int y = 0;

                StackPop(&str_pro->stk1, &x);
                StackPop(&str_pro->stk1, &y);
                $_(x);
                $_(y);

                if (IfCondJump(command, x, y))
                {
                    Do_Jump_if_true(str_pro, buffer, command);
                }
                else
                    str_pro->ip++;
                break;
            }
    /*        switch (command) {

                case ADD:
                case MUL:
                    ...

                case JB:
                case JBE:
                case JA:
                    int x = 0;
                    int y = 0;

                    StackPop(&str_pro->stk1, &x);
                    StackPop(&str_pro->stk1, &y);

                    if (    ) // IfCondJump
    /*
    bool IfCondJump

    switch (command) {
        case JB
            return x < y;
        case JE
            if ()
    }

                    {
                        value_cal = buffer[str_pro->ip];
                        #ifdef DEBUG
                            fprintf(stderr, "STR JUMP: %d\n",value_cal);
                        #endif
                        str_pro->ip = value_cal;

                        #ifdef DEBUG
                            DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__, command);
                        #endif
                    }
                    else
                        str_pro->ip++;

            }*/

            case CALL:
            {
                StackPush(&str_pro->stk_call, str_pro->ip);

                value_cal = buffer[str_pro->ip];
                #ifdef DEBUG
                    fprintf(stderr, "STR CALL: %d\n",value_cal);
                #endif
                str_pro->ip++;

                str_pro->ip = value_cal;

                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__, command);
                #endif
                break;

            }
            case RET:
            {
                int ret_pos = 0;

                StackPop(&str_pro->stk_call, &ret_pos);

                //printf("RET_POS: %d\n", ret_pos);

                str_pro->ip = ret_pos;
                break;
            }
            case PUSHM:
            {
                int curr_register_num = buffer[str_pro->ip];

                str_pro->ip++;

                int* mem_address = getRegisterAddress(str_pro, curr_register_num);

                if (*mem_address >= 0 && *mem_address < 100)
                {
                    int value = str_pro->Ram_Mem [*mem_address];

                    StackPush(&str_pro->stk1, value);

                    printf("PUSHM [%s]: value %d from address %d\n", Get_Name_registr(curr_register_num), value, *mem_address);
                }
                else
                    printf("ERORR, BAD mem_address: %d\n", *mem_address);

                 #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__, __LINE__, command);
                #endif
                break;

            }
            case POPM:
            {
                int curr_register_num = buffer[str_pro->ip];

                str_pro->ip++;

                int* mem_address = getRegisterAddress(str_pro, curr_register_num);

                int value = 0;

                if (*mem_address >= 0 && *mem_address <= 100)
                {
                    StackPop(&str_pro->stk1, &value);

                    str_pro->Ram_Mem [*mem_address] = value;

                    printf("PUSHM [%s]: value %d from address %d\n", Get_Name_registr(curr_register_num), value, *mem_address);
                }
                else
                    printf("ERORR, BAD mem_address: %d\n", *mem_address);

                #ifdef DEBUG
                    DumpPro(str_pro, 0, __FILE__, __func__, __LINE__, command);
                #endif
                break;
            }
            default:
                return false;

        }
    }
    ProcessorDtor (str_pro);
    return 0;
}

int Calculate_func_first (int command, my_pro_t* str_pro)
{
    int x = 0;
    int y = 0;

    StackPop(&str_pro->stk1, &x);
    StackPop(&str_pro->stk1, &y);

    #ifdef DEBUG
        fprintf(stderr, "%d values is:\n%d\n%d\n", x, y, command);
    #endif

    int s = 0;

    switch(command)
    {
        case ADD:
            s = x + y;
            break;
        case SUB:
            s = y - x;
            break;
        case MUL:
            s = x * y;
            break;
        case DIV:
            if (x == 0)
                printf("ERORR: X value is ZERO\n");
            s = y / x;
            break;
        case MOD:
            s = y % x;
            break;
        default:
            return 0;
    }
    return s;
}

void Push_printf (int s, my_pro_t* str_pro, int command)
{
    StackPush(&str_pro->stk1, s);

    #ifdef DEBUG
        fprintf(stderr,"\n\npushed value: %d\n\n", s);
        DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__, command);
    #endif
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


int* getRegisterAddress(my_pro_t* str_pro,int curr_register_num)
{
    switch (curr_register_num)
    {
        case Ax:
            #ifdef DEBUG
                printf("Ax");
            #endif
            return &str_pro->registers[POS_Ax];
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
}

void Do_Jump_if_true(my_pro_t* str_pro, int* buffer, int command)
{
    int value_cal = buffer[str_pro->ip];
    #ifdef DEBUG
        fprintf(stderr, "STR JUMP: %d\n",value_cal);
    #endif
    str_pro->ip = value_cal + 1;

    #ifdef DEBUG
        DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__, command);
    #endif
}

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

void One_value_calc (int command, my_pro_t* str_pro)
{

    assert(str_pro);

    int x = 0;

    StackPop(&str_pro->stk1, &x);

    #ifdef DEBUG
        fprintf(stderr,"%d\n", x);
    #endif

    int s = 0;

    switch(command)
    {
        case POP:
            fprintf(stderr, "POPED VALUE: %d", x);
            break;
        case OUTP:
            printf("\nOUT VALUE: %d\n", x);
            break;
        case POW:
            s = x * x;

            #ifdef DEBUG
                fprintf(stderr, "\n\nPOW value: %d\n", s);
            #endif

            StackPush(&str_pro->stk1, s);

            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__, command);
            #endif
            break;
        case SQRT:
            s = (int) sqrt((double) x);

            StackPush(&str_pro->stk1, s);

            #ifdef DEBUG
                DumpPro(str_pro, 0, __FILE__, __func__ ,__LINE__, command);
            #endif
            break;
        case DISP:
            printf("%c", x);
            break;
        default:
            break;
    }
}
