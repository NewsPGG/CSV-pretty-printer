#include "csv_printer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    const char* input_file = "input.csv";
    const char* output_file = "output.txt";

    if (argc > 1) {
        input_file = argv[1];
    }
    if (argc > 2) {
        output_file = argv[2];
    }

    FILE* in = fopen(input_file, "r");
    if (!in) {
        perror(input_file);
        return 1;
    }
    FILE* out = fopen(output_file, "w");
    if (!out) {
        perror(output_file);
        fclose(in);
        return 1;
    }

    char buffer[1024];
    // Читаем заголовки
    if (!fgets(buffer, sizeof(buffer), in)) {
        fprintf(stderr, "Empty file\n");
        fclose(in);
        fclose(out);
        return 1;
    }
    int num_cols;
    char** headers = parse_csv_line(buffer, &num_cols);

    // Массив для всех строк данных: rows[i] — это массив строк (полей) одной строки
    char*** rows = NULL;
    int num_rows = 0;

    // Читаем остальные строки
    while (fgets(buffer, sizeof(buffer), in)) {
        int cnt;
        char** fields = parse_csv_line(buffer, &cnt);
        // Создаём новую строку с num_cols полями (дополняем пустыми)
        char** row = malloc(num_cols * sizeof(char*));
        for (int i = 0; i < num_cols; i++) {
            if (i < cnt && fields[i])
                row[i] = strdup(fields[i]);
            else
                row[i] = strdup("");
        }
        free_fields(fields);
        // Добавляем в общий массив
        rows = realloc(rows, (num_rows + 1) * sizeof(char**));
        rows[num_rows++] = row;
    }
    fclose(in);

    // Вычисляем ширину каждого столбца
    int* widths = calloc(num_cols, sizeof(int));
    for (int i = 0; i < num_cols; i++) {
        int len = strlen(headers[i]);
        if (len > widths[i]) {
            widths[i] = len;
        }
    }
    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            int len = strlen(rows[r][c]);
            if (len > widths[c]) {
                widths[c] = len;
            }
        }
    }

    // Вывод таблицы
    print_sep(out, '=', widths, num_cols);

    // Заголовки (все влево)
    for (int i = 0; i < num_cols; i++)
        fprintf(out, "| %-*s ", widths[i], headers[i]);
    fprintf(out, "|\n");

    print_sep(out, '=', widths, num_cols);

    // Строки данных
    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            if (is_number(rows[r][c])) {
                fprintf(out, "| %*s ", widths[c], rows[r][c]);  // вправо
            } else {
                fprintf(out, "| %-*s ", widths[c], rows[r][c]); // влево
            }
        }
        fprintf(out, "|\n");
        print_sep(out, '-', widths, num_cols);
    }

    // Освобождаем память
    for (int i = 0; i < num_cols; i++) {
        free(headers[i]);
    }
    free(headers);
    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            free(rows[r][c]);
        }
        free(rows[r]);
    }
    free(rows);
    free(widths);
    fclose(out);
    return 0;
}