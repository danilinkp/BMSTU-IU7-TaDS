#include "table_theatre.h"
#include "menu.h"
#include <stdio.h>

int main(void)
{
    int rc;
    table_theatre_t table_theatre = {.rows = 0};
    theatre_key_t table_keys[MAX_ROW_COUNT];
    size_t size_keys = 0;
    char file_name[256 + 1];
    FILE *file = NULL;
    int action = -1;
    double avg_table_quick_sort, avg_table_insertion_sort, avg_keys_quick_sort, avg_keys_insertion_sort;

    while (action != EXIT_PROGRAM)
    {
        menu_print();
        if (read_int(stdin, &action) != EXIT_SUCCESS)
        {
            printf("Ошибка: Пожалуйста, введите число.\n");
            continue;
        }

        switch (action)
        {
            case EXIT_PROGRAM:
                break;
            case LOAD_TABLE:
                printf("Введите имя файла: ");
                rc = read_str(stdin, file_name, sizeof(file_name));
                if (rc != EXIT_SUCCESS)
                {
                    printf("Ошибка чтения имени файла.\n");
                    break;
                }
                file = fopen(file_name, "r");
                if (!file)
                {
                    printf("Ошибка открытия файла или его не существует.\n");
                    break;
                }
                memset(&table_theatre, 0, sizeof(table_theatre_t));
                size_keys = 0;
                rc = fread_table(&table_theatre, table_keys, &size_keys, file);
                if (rc != EXIT_SUCCESS)
                {
                    fclose(file);
                    printf("Файл записан не правильно, или он пуст.\n");
                    break;
                }
                printf("Таблица успешна загружена.\n");
                break;
            case PRINT_NOT_SORTED_TABLE:
                if (table_theatre.rows == 0)
                {
                    printf("Таблица пуста.\n");
                    break;
                }
                else
                    fprint_table(table_theatre, stdout);
                break;
            case PRINT_NOT_SORTED_KEYS:
                if (size_keys == 0)
                {
                    printf("Таблица ключей пуста.\n");
                    break;
                }
                else
                    fprint_key_table(table_keys, size_keys, stdout);
                break;
            case ADD_THEATRE:
                rc = add_theatre(&table_theatre);
                if (rc != EXIT_SUCCESS)
                {
                    printf("Ошибка добавления театра.\n");
                    break;
                }
                update_keys_with_table(table_theatre, table_keys, &size_keys);
                printf("Театр был успешно добавлен.\n");
                break;
            case DELETE_THEATRE:
                rc = delete_theatre_by_name(&table_theatre);
                if (rc != EXIT_SUCCESS)
                {
                    printf("Ничего не удалилось.\n");
                    break;
                }
                printf("Театр успешно удален.\n");
                update_keys_with_table(table_theatre, table_keys, &size_keys);
                break;
            case PRINT_SORTED_KEYS:
                if (size_keys == 0)
                {
                    printf("Таблица ключей пуста.\n");
                    break;
                }
                qsort(table_keys, size_keys, sizeof(theatre_key_t), key_comp);
                fprint_key_table(table_keys, size_keys, stdout);
                break;
            case PRINT_SORTED_TABLE:
                if (table_theatre.rows == 0)
                    printf("Таблица пуста.\n");
                qsort(table_theatre.theatres, table_theatre.rows, sizeof(theatre_t), theatre_t_comp);
                fprint_table(table_theatre, stdout);
                update_keys_with_table(table_theatre, table_keys, &size_keys);
                break;
            case PRINT_SORTED_TABLE_BY_KEYS:
                if (size_keys == 0)
                {
                    printf("Таблицы пусты.\n");
                    break;
                }
                qsort(table_keys, size_keys, sizeof(theatre_key_t), key_comp);
                fprint_table_by_keys(table_theatre, table_keys, stdout);
                break;
            case PRINT_BALLETS:
                printf("Введите возрастное ограничение на балет (возраст должен быть больше или равен 3): ");
                int age;

                if (read_int(stdin, &age) != EXIT_SUCCESS || age < 3)
                {
                    printf("Ошибка ввода возраста.\n");
                    break;
                }
                printf("Введите продолжительность балета: ");
                int duration;
                if (read_int(stdin, &duration) != EXIT_SUCCESS || duration <= 0)
                {
                    printf("Ошибка ввода продолжительности.\n");
                    break;
                }
                rc = find_ballets(table_theatre, age, duration);
                if (rc != EXIT_SUCCESS)
                {
                    printf("Балеты не найдены.\n");
                    break;
                }
                break;
            case COMP_TABLE_WITH_KEYS:
                if (table_theatre.rows == 0)
                {
                    printf("Таблицы пусты. Замерять нечего.\n");
                    break;
                }
                printf("Таблица времени от сортировки и алгоритма работы:\n");
                avg_table_quick_sort = time_sort_table(table_theatre, NUM_OF_ITERATIONS, qsort);
                avg_keys_quick_sort = time_sort_keys(table_keys, size_keys, NUM_OF_ITERATIONS, qsort);

                avg_table_insertion_sort = time_sort_table(table_theatre, NUM_OF_ITERATIONS, insertion_sort);
                avg_keys_insertion_sort = time_sort_keys(table_keys, size_keys, NUM_OF_ITERATIONS, insertion_sort);

                printf("| %-25s | %-25s | %-14s |\n", "Algorithm", "Data structure", "Avg time");
                printf("|---------------------------|---------------------------|----------------|\n");
                printf("| %-25s | %-25s | %-14lf |\n", "Quick Sort", "Table", avg_table_quick_sort);
                printf("| %-25s | %-25s | %-14lf |\n", "Quick Sort", "Keys", avg_keys_quick_sort);
                printf("|---------------------------|---------------------------|----------------|\n");
                printf("| %-25s | %-25s | %-14lf |\n", "Insertion Sort", "Table", avg_table_insertion_sort);
                printf("| %-25s | %-25s | %-14lf |\n\n", "Insertion Sort", "Keys", avg_keys_insertion_sort);
                printf(" -Quick sort: КЛЮЧИ на %.2lf%% более эффективны, чем сортировка ТАБЛИЦЫ\n",
                       (avg_table_quick_sort - avg_keys_quick_sort) / avg_table_quick_sort * 100);
                printf(" -Insertion sort: КЛЮЧИ на  %.2lf%% более эффективны, чем сортировка ТАБЛИЦЫ\n",
                       (avg_table_insertion_sort - avg_keys_insertion_sort) / avg_table_insertion_sort * 100);

                printf("\nРазмеры: \n");
                printf(" -Таблицы: %zu байт\n", sizeof(theatre_t) * table_theatre.rows);
                printf(" -Таблицы + ключи: %zu байт\n", sizeof(theatre_t) * table_theatre.rows + sizeof(theatre_key_t) * size_keys);
                printf(" -Ключи: %zu байт\n", sizeof(theatre_key_t) * size_keys);
                printf("Таблица с ключами занимает на %.2lf%% больше места, чем сама таблица\n",
                       ((double) (sizeof(theatre_t) * table_theatre.rows + sizeof(theatre_key_t) * size_keys)) /
                       ((double) (sizeof(theatre_t) * table_theatre.rows)) * 100 - 100);
                break;
            case COMPARE_SORTS:
                if (table_theatre.rows == 0)
                {
                    printf("Таблицы пусты. Замерять нечего.\n");
                    break;
                }
                printf("Таблица времени от сортировки и алгоритма работы:\n");
                avg_table_quick_sort = time_sort_table(table_theatre, NUM_OF_ITERATIONS, qsort);
                avg_keys_quick_sort = time_sort_keys(table_keys, size_keys, NUM_OF_ITERATIONS, qsort);

                avg_table_insertion_sort = time_sort_table(table_theatre, NUM_OF_ITERATIONS, insertion_sort);
                avg_keys_insertion_sort = time_sort_keys(table_keys, size_keys, NUM_OF_ITERATIONS, insertion_sort);

                printf("| %-25s | %-25s | %-14s |\n", "Data structure", "Algorithm", "Avg time");
                printf("|---------------------------|---------------------------|----------------|\n");
                printf("| %-25s | %-25s | %-14lf |\n", "Table", "Quick Sort", avg_table_quick_sort);
                printf("| %-25s | %-25s | %-14lf |\n", "", "Insertion Sort", avg_table_insertion_sort);
                printf("|---------------------------|---------------------------|----------------|\n");
                printf("| %-25s | %-25s | %-14lf |\n", "Keys", "Quick Sort", avg_keys_quick_sort);
                printf("| %-25s | %-25s | %-14lf |\n\n", "", "Insertion Sort", avg_keys_insertion_sort);

                printf("ЭФФЕКТИВНОСТЬ СОРТИРОВКИ ДЛЯ ТАБЛЦИЦЫ -QUICK sort на %.2lf%% более эффективен, чем INSERTION sort для Таблиц\n",
                       (avg_table_insertion_sort - avg_table_quick_sort) / avg_table_insertion_sort * 100);
                if (avg_keys_insertion_sort < avg_keys_quick_sort)
                    printf("ЭФФЕКТИВНОСТЬ СОРТИРОВКИ ДЛЯ КЛЮЧЕЙ -INSERTION sort на %.2lf%% более эффективен, чем QUICK sort для Ключей\n",
                           (avg_keys_quick_sort - avg_keys_insertion_sort) / avg_keys_quick_sort * 100);
                else
                    printf(" -Qsort на %.2lf%% более эффективен, чем сортировка вставками для Ключей\n",
                           (avg_keys_insertion_sort - avg_keys_quick_sort) / avg_keys_insertion_sort * 100);

                printf("\nРазмеры: \n");
                printf(" -Таблицы: %zu байт\n", sizeof(theatre_t) * table_theatre.rows);
                printf(" -Таблицы + ключи: %zu байт\n", sizeof(theatre_t) * table_theatre.rows + sizeof(theatre_key_t) * size_keys);
                printf(" -Ключи: %zu байт\n", sizeof(theatre_key_t) * size_keys);
                printf("Таблица с ключами занимает на %.2lf%% больше места, чем сама таблица\n",
                       ((double) (sizeof(theatre_t) * table_theatre.rows + sizeof(theatre_key_t) * size_keys)) /
                       ((double) (sizeof(theatre_t) * table_theatre.rows)) * 100 - 100);
                break;

            case SAVE_FILE:
                printf("Введите имя файла, в который хотите сохранить таблицу: ");
                rc = read_str(stdin, file_name, sizeof(file_name));
                if (rc != EXIT_SUCCESS)
                {
                    printf("Ошибка чтения имени файла.\n");
                    break;
                }
                file = fopen(file_name, "w");
                if (!file)
                {
                    printf("Ошибка открытия файла или его не существует.\n");
                    break;
                }
                save_file(table_theatre, file);

                printf("Таблца успешно сохранена.\n");
                break;
            default:
                printf("Выберите 1 из пунктов меню.\n");
                break;
        }
    }
    return EXIT_SUCCESS;
}
