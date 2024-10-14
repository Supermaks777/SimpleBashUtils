CC = gcc
CFLAGS = -Wall -Werror -Wextra
all : s21_grep
clean : 
	rm -rf *.o s21_grep
rebuild : clean all

s21_grep.o :
	$(CC) $(CFLAGS) -c s21_grep.c
s21_grep : s21_grep.o
	$(CC) -o s21_grep s21_grep.o

valgrind: s21_grep
	# valgrind --tool=memcheck --leak-check=yes  sh test_grep.sh
	valgrind --leak-check=full --show-leak-kinds=all sh grep_test.sh

docker:
	docker build -t "container" .
	docker run -it -v $(shell pwd):/tmp -w /tmp container