all:
	gcc -g -shared -fPIC -o libmy_execve.so my_execve.c
	LD_PRELOAD=./libmy_execve.so ./cmd3.sh
