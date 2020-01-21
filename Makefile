all:
	gcc server_all.c -pthread
	./a.out
	gcc client_all.c -pthread
	./a.out