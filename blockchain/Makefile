CC       = gcc
FLAGS    = -Wall -Wextra
OBJ_FILE = main.o blockchain.o block.o
EXE_FILE = main

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS} -o ${EXE_FILE}  ${OBJ_FILE}
	rm *.o

main.o: main.c
	${CC} -c main.c

blockchain.o: blockchain.c
	${CC} -c blockchain.c

block.o: block.c
	${CC} -c block.c

crypto.o: crypto.c
	${CC} -c crypto.c

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}
