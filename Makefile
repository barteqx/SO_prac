Flags = --std=gnu99 -D_SVID_SOURCE=700 -pthread -lrt -lc

main: b64.o operations.o main.o
	gcc b64.o operations.o main.o -o main $(Flags)

main.o: zad1.c 
	gcc -c zad1.c -o main.o $(Flags)

operations.o: operations.c operations.h b64.h
	gcc -c operations.c -o operations.o $(Flags)

b64.o: b64.c b64.h
	gcc -c b64.c -o b64.o $(Flags)