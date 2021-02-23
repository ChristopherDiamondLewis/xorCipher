CC = gcc -Wextra -Wall


all: encryptUtil.o main.o  
			$(CC) encryptUtil.o main.o -o encryptUtil

encryptUtil.o: encryptUtil.c encryptUtil.h
			$(CC) -c encryptUtil.c

main.o: main.c encryptUtil.h
			$(CC) -c main.c

clean:
			rm -f *.o encryptUtil