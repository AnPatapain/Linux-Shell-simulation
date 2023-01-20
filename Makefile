main: src/mini_string.c src/mini_io.c src/mini_memory.c src/main.c
	gcc src/mini_io.c src/mini_memory.c src/mini_string.c src/mini_shell.c src/main.c -o main
