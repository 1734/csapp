all:
	gcc -g -shared -fPIC -o libmy_execve.so my_execve.c -ldl
	MY_PRELOAD_SO="./libmy_execve.so" LD_PRELOAD=./libmy_execve.so ./cmd3.sh
