CC=gcc
CFLAGS=-c -g -Wall -Werror
INC_DIR=-I./
LIBRARY=-linfra

all: rbtree_demo lookup_table indexer

rbtree_demo: demo.o rbtree.o
	${CC} -o $@ $^

lookup_table: lookup_table.o rbtree.o
	${CC} -o $@ $^

indexer: indexer.o rbtree.o
	${CC} -o $@ $^

%.o: %.c
	${CC} ${CFLAGS} ${INC_DIR} $^

clean:
	rm -f *.o rbtree_demo lookup_table indexer
