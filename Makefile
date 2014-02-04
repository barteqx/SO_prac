Flags = --std=c99 -D_SVID_SOURCE=700 -Wall -Wpedantic -pthread

main: b64.o operations.o main.o
	gcc $(Flags) b64.o operations.o main.o -o main

main.o: zad1.c 
	gcc $(Flags) -c zad1.c -o main.o

operations.o: operations.c operations.h b64.h
	gcc $(Flags) -c operations.c -o operations.o

b64.o: b64.c b64.h
	gcc $(Flags) -c b64.c -o b64.o