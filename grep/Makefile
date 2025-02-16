CC=gcc
CFLAGS=-c -Wall  -Wextra -Werror

all: s21_grep grep_test 

s21_grep: grep.o s21_grep.o list.o grep_file_regex.o
	$(CC) -o s21_grep grep.o s21_grep.o list.o grep_file_regex.o
grep.o: grep.c
	$(CC) $(CFLAGS) grep.c
s21_grep.o: s21_grep.c 
	$(CC) $(CFLAGS) s21_grep.c
list.o: list.c
	$(CC) $(CFLAGS) list.c
grep_file_regex.o: grep_file_regex.c
	$(CC) $(CFLAGS) grep_file_regex.c
clean:
	rm -rf *.o
	rm -rf s21_grep