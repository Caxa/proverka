#ifndef S21_GREP_H
#define S21_GREP_H

void grep_file(const char *filename, const char *pattern, int flags[]);
void parse_arguments(int argc, char *argv[], char **pattern, char **file,
                     int flags[]);

#endif  // S21_GREP_H
