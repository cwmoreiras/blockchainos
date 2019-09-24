CC       = gcc
CFLAGS   = -g
LFLAGS   = -Wall -Wextra
OBJ_FILE = main.o blockchain.o block.o crypto.o
EXE_FILE = main

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  ${OBJ_FILE} -o ${EXE_FILE}
	rm *.o

main.o: main.c
	${CC} ${LFLAGS} -c main.c

blockchain.o: blockchain.c
	${CC} ${FLAGS} -c blockchain.c

block.o: block.c
	${CC} ${LFLAGS} -c block.c

crypto.o: crypto.c
	${CC} ${LFLAGS} -c crypto.c

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}
