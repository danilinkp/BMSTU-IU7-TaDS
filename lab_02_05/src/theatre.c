#include "theatre.h"
#include "tools.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Функции чтения
 */
int read_prices(int *low_price, int *high_price, FILE *file_input)
{
    if (file_input == stdin)
        printf("Введите минимальную и максимальную стоимость билета через пробел\n");
    if (read_two_ints(file_input, low_price, high_price) != EXIT_SUCCESS)
        return WRONG_PRICES_ERROR;
    return EXIT_SUCCESS;
}

int read_performance_type(performance_type_t *performance_type, FILE *file_input)
{
    int tmp;
    if (file_input == stdin)
        printf("Выберите тип спектакля:\n1) Пьеса\n2) Драма \n3) Комедия\n4) Сказка\n5) Музыкальный.\n");
    if (read_int(file_input, &tmp) != EXIT_SUCCESS)
        return WRONG_TYPE_PERFORMANCE_ERROR;

    if (tmp < 1 || tmp > 5)
        return WRONG_TYPE_PERFORMANCE_ERROR;

    *performance_type = tmp;

    return EXIT_SUCCESS;
}

int read_age_limit(age_limit_t *age_limit, FILE *file_input)
{
    int tmp;
    if (file_input == stdin)
        printf("Введите возрастное ограничение на спектакль:\n1) 3+\n2) 10+\n3) 16+\n");
    if (read_int(file_input, &tmp) != EXIT_SUCCESS)
        return WRONG_AGE_LIMIT_ERROR;

    if (tmp < 1 || tmp > 3)
        return WRONG_AGE_LIMIT_ERROR;

    *age_limit = tmp;

    return EXIT_SUCCESS;
}

int read_musical_type(musical_type_t *musical_type, FILE *file_input)
{
    int tmp;
    if (file_input == stdin)
        printf("Выберите тип музыкального спектакля:\n1) Балет\n2) Опера\n3) Мюзикл\n");
    if (read_int(file_input, &tmp) != EXIT_SUCCESS)
        return WRONG_MUSICAL_TYPE_ERROR;

    if (tmp < 1 || tmp > 3)
        return WRONG_MUSICAL_TYPE_ERROR;

    *musical_type = tmp;

    return EXIT_SUCCESS;
}

int read_musical(musical_t *musical, FILE *file_input)
{
    if (file_input == stdin)
        printf("Введите имя копозитора (максимальное кол-во символов %d):\n", MAX_STR_LEN);
    if (read_str(file_input, musical->composer, MAX_STR_LEN) != EXIT_SUCCESS)
        return MUSICAL_READ_ERROR;

    if (file_input == stdin)
        printf("Введите название страны (максимальное кол-во символов %d):\n", MAX_STR_LEN);
    if (read_str(file_input, musical->country, MAX_STR_LEN) != EXIT_SUCCESS)
        return MUSICAL_READ_ERROR;

    int rc = read_musical_type(&musical->type, file_input);
    if (rc != EXIT_SUCCESS)
        return MUSICAL_READ_ERROR;

    rc = read_age_limit(&musical->age, file_input);
    if (rc != EXIT_SUCCESS)
        return MUSICAL_READ_ERROR;

    if (file_input == stdin)
        printf("Введите продолжительность спектакля в минутах (больше нуля):\n");
    int tmp;
    if (read_int(file_input, &tmp) != EXIT_SUCCESS)
        return MUSICAL_READ_ERROR;
    if (tmp < 0)
        return MUSICAL_READ_ERROR;
    musical->duration = tmp;

    return EXIT_SUCCESS;
}

int read_performance(theatre_t *theatre, FILE *file_input)
{
    int rc = EXIT_SUCCESS;
    int performance_type = theatre->performance_type;
    switch (performance_type)
    {
        case MUSICAL:
            rc = read_musical(&theatre->performance.musical, file_input);
            break;
        case FAIRY_TALE:
            rc = read_age_limit(&theatre->performance.fairy_tale_age_limit, file_input);
            break;
        default:
            break;
    }
    return rc;
}

int read_theatre(theatre_t *theatre, FILE *file_input)
{
    int rc = EXIT_SUCCESS;
    do
    {
        if (file_input == stdin)
            printf("Введите название театра (не более %d):\n", MAX_STR_LEN);
        rc = read_str(file_input, theatre->name, MAX_STR_LEN);
        if (rc != EXIT_SUCCESS)
        {
            if (file_input == stdin)
                printf("Ошибка ввода названия тетара\n");
            else
                return rc;
        }
    } while (rc != EXIT_SUCCESS);

    do
    {
        if (file_input == stdin)
            printf("Введите название спектакля (не более %d):\n", MAX_STR_LEN);
        rc = read_str(file_input, theatre->name, MAX_STR_LEN);
        if (rc != EXIT_SUCCESS)
        {
            if (file_input == stdin)
                printf("Ошибка ввода названия спектакля\n");
            else
                return rc;
        }
    } while (rc != EXIT_SUCCESS);

    do
    {
        rc = read_prices(&theatre->price_low, &theatre->price_high, file_input);
        if (rc != EXIT_SUCCESS)
        {
            if (file_input == stdin)
                printf("Ошибка ввода диапозона цен\n");
            else
                return rc;
        }
    } while (rc != EXIT_SUCCESS);

    do
    {
        rc = read_performance_type(&theatre->performance_type, file_input);
        if (rc != EXIT_SUCCESS)
        {
            if (file_input == stdin)
                printf("Ошибка ввода типа спектакля\n");
            else
                return rc;
        }
    } while (rc != EXIT_SUCCESS);

    do
    {
        rc = read_performance(theatre, file_input);
        if (rc != EXIT_SUCCESS)
        {
            if (file_input == stdin)
                printf("Ошибка вводам параметров спетакля\n");
            else
                return rc;
        }
    } while (rc != EXIT_SUCCESS);

    return EXIT_SUCCESS;
}

/*
 * Функции вывода
 */
void print()
