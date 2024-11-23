#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Прототипы функций
void grep_file(const char *filename, const char *pattern, int flags[]);
void parse_arguments(int argc, char *argv[], char **pattern, char **file,
                     int flags[]);
char *read_patterns_from_file(const char *filename);

enum {
  FLAG_E = 0,
  FLAG_I,
  FLAG_V,
  FLAG_C,
  FLAG_L,
  FLAG_N,
  FLAG_H,  // Не показывать имя файла
  FLAG_S,  // Подавить сообщения об ошибках
  FLAG_F,  // Загрузка шаблонов из файла
  FLAG_O,  // Показать только совпадающие части строк
  FLAG_COUNT
};

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s [flags] <pattern> <file>\n", argv[0]);
    return 1;
  }

  int flags[FLAG_COUNT] = {0};
  char *pattern = NULL;
  char *filename = NULL;

  parse_arguments(argc, argv, &pattern, &filename, flags);

  if (flags[FLAG_F]) {
    pattern = read_patterns_from_file(pattern);
    if (!pattern) {
      fprintf(stderr, "Error: Unable to load patterns from file\n");
      return 1;
    }
  }

  if (!pattern || !filename) {
    fprintf(stderr, "Error: Missing pattern or file\n");
    return 1;
  }

  grep_file(filename, pattern, flags);

  if (flags[FLAG_F]) free(pattern);  // Освобождаем память, если использовали -f
  return 0;
}

void parse_arguments(int argc, char *argv[], char **pattern, char **file,
                     int flags[]) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      for (int j = 1; argv[i][j]; j++) {
        switch (argv[i][j]) {
          case 'e':
            flags[FLAG_E] = 1;
            break;
          case 'i':
            flags[FLAG_I] = 1;
            break;
          case 'v':
            flags[FLAG_V] = 1;
            break;
          case 'c':
            flags[FLAG_C] = 1;
            break;
          case 'l':
            flags[FLAG_L] = 1;
            break;
          case 'n':
            flags[FLAG_N] = 1;
            break;
          case 'h':
            flags[FLAG_H] = 1;
            break;
          case 's':
            flags[FLAG_S] = 1;
            break;
          case 'f':
            flags[FLAG_F] = 1;
            break;
          case 'o':
            flags[FLAG_O] = 1;
            break;
          default:
            fprintf(stderr, "Unknown flag: -%c\n", argv[i][j]);
            exit(1);
        }
      }
    } else {
      if (!*pattern) {
        *pattern = argv[i];
      } else {
        *file = argv[i];
      }
    }
  }
}
char *read_patterns_from_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening patterns file");
    return NULL;
  }

  char *patterns = malloc(BUFFER_SIZE);
  if (!patterns) {
    perror("Memory allocation error");
    fclose(file);
    return NULL;
  }

  patterns[0] = '\0';
  char line[BUFFER_SIZE];
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = '\0';  // Убираем перенос строки
    strcat(patterns, line);
    strcat(patterns, "|");  // Разделяем шаблоны
  }
  if (strlen(patterns) > 0)
    patterns[strlen(patterns) - 1] = '\0';  // Убираем последний '|'

  fclose(file);
  return patterns;
}

void grep_file(const char *filename, const char *pattern, int flags[]) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    if (!flags[FLAG_S]) perror("Error opening file");
    return;
  }

  regex_t regex;
  int reg_flags = REG_EXTENDED | (flags[FLAG_I] ? REG_ICASE : 0);
  if (regcomp(&regex, pattern, reg_flags)) {
    if (!flags[FLAG_S])
      fprintf(stderr, "Error compiling regex for pattern: %s\n", pattern);
    fclose(file);
    return;
  }

  char line[BUFFER_SIZE];
  int line_num = 0;
  int match_count = 0;
  int has_match = 0;

  // Отладка
  fprintf(stderr, "Regex compiled successfully: %s\n", pattern);

  while (fgets(line, sizeof(line), file)) {
    line_num++;
    int matched = !regexec(&regex, line, 0, NULL, 0);
    if (flags[FLAG_V]) matched = !matched;

    fprintf(stderr, "Line %d: '%s' Matched: %d\n", line_num, line, matched);

    if (matched) {
      has_match = 1;
      match_count++;

      if (flags[FLAG_C]) continue;
      if (flags[FLAG_L]) break;

      if (flags[FLAG_N]) printf("%d:", line_num);
      if (flags[FLAG_O]) {
        regmatch_t match;
        const char *p = line;
        while (!regexec(&regex, p, 1, &match, 0)) {
          printf("%.*s\n", (int)(match.rm_eo - match.rm_so), p + match.rm_so);
          p += match.rm_eo;
          if (match.rm_eo == match.rm_so) break;  // Избежание зацикливания
        }
        continue;
      }
      printf("%s", line);
      fprintf(stderr, "Printed line: %s\n", line);
    }
  }

  if (flags[FLAG_C]) {
    printf("%d\n", match_count);
    fprintf(stderr, "Count printed: %d\n", match_count);
  }

  if (flags[FLAG_L] && has_match) {
    printf("%s\n", filename);
    fprintf(stderr, "Filename printed: %s\n", filename);
  }

  regfree(&regex);
  fclose(file);
}
