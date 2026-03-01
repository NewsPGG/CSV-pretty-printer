#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Разбор одной строки csv-файла
char** parse_csv_line(char* line, int* out_count)
{
    // Убираем \n в конце
    line[strcspn(line, "\r\n")] = 0;

    // Считаем количество запятых
    int commas = 0;
    for (char* p = line; *p; p++)
        if (*p == ',') {
            commas++;
        }
    int count = commas + 1;
    *out_count = count;

    char** fields = malloc((count + 1) * sizeof(char*));
    int idx = 0;
    char* start = line;
    for (char* p = line;; p++) {
        if (*p == ',' || *p == '\0') {
            int len = p - start;
            char* field = malloc(len + 1);
            strncpy(field, start, len);
            field[len] = '\0';
            fields[idx++] = field;
            start = p + 1;
            if (*p == '\0') {
                break;
            }
        }
    }
    fields[idx] = NULL;
    return fields;
}

// Функция для освобождения памяти
void free_fields(char** fields)
{
    if (!fields) {
        return;
    }
    for (int i = 0; fields[i]; i++) {
        free(fields[i]);
    }
    free(fields);
}

// Функция проверки на число
int is_number(const char* str)
{
    if (!str || !*str) {
        return 0;
    }
    char* endptr;
    strtod(str, &endptr);
    while (*endptr && isspace((unsigned char)*endptr)) {
        endptr++;
    }
    return *endptr == '\0';
}

// Печатает разделительную линию
void print_sep(FILE* out, char ch, int* widths, int num_cols)
{
    for (int i = 0; i < num_cols; i++) {
        fputc('+', out);
        for (int j = 0; j < widths[i] + 2; j++) {
            fputc(ch, out);
        }
    }
    fputc('+', out);
    fputc('\n', out);
}