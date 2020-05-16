all:
	gcc -w main.c -o main -fsanitize=address