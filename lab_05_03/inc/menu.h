#ifndef LAB_05_03_MENU_H
#define LAB_05_03_MENU_H

typedef enum
{
    EXIT,
    ARR_QUEUE_PUSH,
    LIST_QUEUE_PUSH,
    ARR_QUEUE_POP,
    LIST_QUEUE_POP,
    PRINT_ARR_QUEUE,
    PRINT_LIST_QUEUE,
    PRINT_FREE_LIST,
    MAKE_SIMULATION_ON_ARR,
    MAKE_SIMULATION_ON_LIST,
    CHANGE_PARAMETRS,
    PRINT_MEASUREMENTS
} menu_t;

enum menu_errors
{
    WRONG_VALUE_READ_ERROR = 11
};

void menu_print(void);
#endif //LAB_05_03_MENU_H
