GCC_CFLAGS = gcc -Wall -Werror -Wextra

all: s21_grep

s21_grep: s21_grep.o 
	$(GCC_CFLAGS) s21_grep.o -o s21_grep

grep.o: s21_grep.c
	$(GCC_CFLAGS) -c s21_grep.c

test: t_grep.sh
	make all
	bash t_grep.sh

clean:
	rm -f s21_grep.o s21_grep