#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "CatFlags.h"

typedef struct {
  bool numberNonBlank;
  bool markEndl;
  bool numberAll;
  bool squeeze;
  bool tab;
  bool printNonPrintable;

} Flags;

Flags CatReadFlags(int argc, char *argv[]) {
  struct option longOptions[] = {
      {"number-nonblank", 0, NULL, 'b'},
      {"number", 0, NULL, 'n'},
      {"squeeze-blank", 0, NULL, 's'},
      {NULL, 0, NULL, 0}};

  int currntFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL);
  Flags flags = {false, false, false, false, false, false};
  for (; currntFlag != -1;
       currntFlag = getopt_long(argc, argv, "bevEnstT", longOptions, NULL)) {
    switch (currntFlag) {
      case 'b':
        flags.numberNonBlank = true;
        break;
      case 'e':
        flags.markEndl = true;
        flags.printNonPrintable = true;
        break;
      case 'v':
        flags.printNonPrintable = true;
        break;
      case 'E':
        flags.markEndl = true;  // Устанавливаем флаг добавления $ в конце строки
        break;
      case 'n':
        flags.numberAll = true;
        break;
      case 's':
        flags.squeeze = true;
        break;
      case 't':
        flags.printNonPrintable = true;
        flags.tab = true;
        break;
      case 'T':
        flags.tab = true;
        break;
    }
  }
  return flags;
}
void CatFile(FILE *file, Flags flags, const char *table[static 256]) {
  int c = 0;
  int last = '\n';
  bool squeeze = false;
  int lineno = 0;

  while (fread(&c, 1, 1, file) > 0) {
    if (last == '\n') {
      if (flags.squeeze && c == '\n') {
        if (squeeze) continue;
        squeeze = true;
      } else {
        squeeze = false;
      }

      if (flags.numberNonBlank) {
        if (c != '\n') printf("%6i\t", ++lineno);
      } else if (flags.numberAll) {
        printf("%6i\t", ++lineno);
      }
    }

    if (c == '\n' && flags.markEndl) {
      printf("$\n");  // Добавляем $ перед новой строкой
    } else {
      printf("%s", table[c]);
    }

    last = c;
  }
}
/*void CatFile(FILE *file, Flags flags, const char *table[static 256]) {
  int c = 0;
  int last;
  bool squeeze =false;
  last = '\n';
  (void)flags;
  int lineno = 0;
  while (fread(&c, 1, 1, file) > 0) {
    if (last == '\n') {
      if (flags.squeeze && c == '\n') {
        if (squeeze) continue;
        squeeze = true;
      }
      else
      squeeze= false;
      if (flags.numberNonBlank) {
        if (c != '\n') printf("%6i\t", ++lineno);
      } else if (flags.numberAll) {
        printf("%6i\t", ++lineno);
      }
    }
    if (!*table[c])
      printf("%c", '\0');
    else
      printf("%s", table[c]);
    last = c;
  }
}*/

void Cat(int argc, char *argv[], Flags flags, const char *table[static 256]) {
  for (char **filename = &argv[1], **end = &argv[argc]; filename != end;
       ++filename) {
    if (**filename == '-') {
      continue;
    }
    FILE *file = fopen(*filename, "rb");
    if (errno) {
      fprintf(stderr, "%s", argv[0]);
      perror(*filename);
      continue;
    }
    CatFile(file, flags, table);
    fclose(file);
  }
}
int main(int argc, char **argv) {
  Flags flags = CatReadFlags(argc, argv);
  const char *table[256];
  CatSetTable(table);
  if (flags.markEndl) {
    catSetEndl(table);
  }
  if (flags.tab) {
    catSetTab(table);
  }
  if (flags.printNonPrintable) {
    catSetNonPrintable(table);
  }
  Cat(argc, argv, flags, table);
  return 0;
}