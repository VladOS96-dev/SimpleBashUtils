CC=gcc
CFLAGS=-c -Wall  -Wextra -Werror
all: s21_cat 
s21_cat: s21_cat.o cat.o 
	$(CC) -o s21_cat s21_cat.o cat.o 
	
s21_cat.o: s21_cat.c
	$(CC) $(CFLAGS) s21_cat.c

cat_test.o:cat_test.c
	$(CC) $(CFLAGS) cat_test.c

cat.o: cat.c
	$(CC) $(CFLAGS) cat.c

clean: 
	rm -rf *.o
	rm -rf s21_cat
