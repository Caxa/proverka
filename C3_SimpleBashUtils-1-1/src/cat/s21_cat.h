#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Структура для флагов
typedef struct {
    int number_lines;
    int number_nonblank;
    int show_ends;
    int squeeze_blank;
    int show_tabs;
    int show_nonprint;
} Flags;

Flags parse_arguments(int argc, char *argv[]);
void print_file(FILE *file, Flags *flags);

#endif // S21_CAT_H
