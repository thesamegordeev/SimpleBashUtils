CC = gcc -Wall -Werror -Wextra

cat : cat.c cat_func.c
	$(CC) cat.c cat_func.c -o cat

rebuild :
	make clean
	make cat

clean :
	rm cat
