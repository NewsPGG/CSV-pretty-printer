#pragma once
#include <stdio.h>

// Прототипы (описание функция в csv_printer.c)
char** parse_csv_line(char* line, int* out_count);
void free_fields(char** fields);
int is_number(const char* str);
void print_sep(FILE* out, char ch, int* widths, int num_cols);