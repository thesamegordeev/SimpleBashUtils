CC = gcc -Wall -Werror -Wextra

grep: grep.c grep_func.c 
	$(CC) grep.c grep_func.c -o grep

rebuild :
	make clean
	make grep

clean :
	rm grep
