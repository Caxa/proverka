# Определение компилятора и флагов
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

# Директории для s21_cat и s21_grep
CAT_SRC_DIR = cat
CAT_BIN_DIR = cat
GREP_SRC_DIR = grep
GREP_BIN_DIR = grep

# Объекты для s21_cat
CAT_OBJ = $(CAT_SRC_DIR)/s21_cat.o

# Объекты для s21_grep
GREP_OBJ = $(GREP_SRC_DIR)/s21_grep.o

# Цель по умолчанию
all: $(CAT_BIN_DIR)/s21_cat $(GREP_BIN_DIR)/s21_grep

# Сборка исполняемого файла для s21_cat
$(CAT_BIN_DIR)/s21_cat: $(CAT_OBJ)
	$(CC) $(CFLAGS) -o $@ $(CAT_OBJ)

# Компиляция s21_cat.c
$(CAT_SRC_DIR)/s21_cat.o: $(CAT_SRC_DIR)/s21_cat.c $(CAT_SRC_DIR)/s21_cat.h
	$(CC) $(CFLAGS) -c $< -o $@

# Сборка исполняемого файла для s21_grep
$(GREP_BIN_DIR)/s21_grep: $(GREP_OBJ)
	$(CC) $(CFLAGS) -o $@ $(GREP_OBJ) -lpcre

# Компиляция s21_grep.c
$(GREP_SRC_DIR)/s21_grep.o: $(GREP_SRC_DIR)/s21_grep.c $(GREP_SRC_DIR)/s21_grep.h
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(CAT_OBJ) $(CAT_BIN_DIR)/s21_cat
	rm -f $(GREP_OBJ) $(GREP_BIN_DIR)/s21_grep
