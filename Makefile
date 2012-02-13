all:
	gcc -Wall -pedantic client.c -o client
	gcc -Wall -pedantic server.c -o server
