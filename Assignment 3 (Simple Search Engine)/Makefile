# Makefile for programs to index and search an index.

FLAGS= -Wall -g
SRC =  freq_list.c punc.c
OBJ =  freq_list.o punc.o

all : indexer queryone printindex query test_worker

indexer : indexer.o ${OBJ}
	gcc ${FLAGS} -o $@ indexer.o ${OBJ}

printindex : printindex.o ${OBJ}
	gcc ${FLAGS} -o $@ printindex.o ${OBJ}

queryone : queryone.o worker.o ${OBJ}
	gcc ${FLAGS} -o $@ queryone.o worker.o ${OBJ}

query : query.o worker.o ${OBJ}
	gcc ${FLAGS} -o $@ query.o worker.o ${OBJ}

test_worker : test_worker.o worker.o ${OBJ}
	gcc ${FLAGS} -o $@ test_worker.o worker.o ${OBJ}

# Separately compile each C file
%.o : %.c freq_list.h 
	gcc ${FLAGS} -c $<

queryone.o : worker.h
worker.o : worker.h
query.o : worker.h

clean :
	-rm *.o indexer queryone printindex query test_worker
