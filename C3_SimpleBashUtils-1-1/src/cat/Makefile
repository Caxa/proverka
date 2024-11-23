GCC_CFLAGS = gcc -Wall -Werror -Wextra

all: s21_cat

s21_cat: s21_cat.o 
	$(GCC_CFLAGS) s21_cat.o -o s21_cat

cat.o: s21_cat.c
	$(GCC_CFLAGS) -c s21_cat.c

test: t_cat.sh
	make all
	bash t_cat.sh

clean:
	rm -f s21_cat.o s21_cat