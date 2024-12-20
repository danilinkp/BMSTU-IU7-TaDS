#ifndef LAB_02_05_MENU_H
#define LAB_02_05_MENU_H

typedef enum
{
    EXIT_PROGRAM,
    LOAD_TABLE,
    PRINT_NOT_SORTED_TABLE,
    PRINT_NOT_SORTED_KEYS,
    ADD_THEATRE,
    DELETE_THEATRE,
    PRINT_SORTED_KEYS,
    PRINT_SORTED_TABLE,
    PRINT_SORTED_TABLE_BY_KEYS,
    PRINT_BALLETS,
    COMP_TABLE_WITH_KEYS,
    COMPARE_SORTS,
    SAVE_FILE
} menu_action_t;

void menu_print(void);

#endif //LAB_02_05_MENU_H
