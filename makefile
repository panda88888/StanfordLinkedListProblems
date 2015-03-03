CC=gcc
CFLAGS=-Wall

all:
	echo Compiling StanfordLinkedListProblem.c
	$(CC) $(CFLAGS) -o hello StanfordLinkedListProblem.c