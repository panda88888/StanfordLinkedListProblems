cc=gcc
cflags=-Wall

all:
	echo Compiling StanfordLinkedListProblem.c
	$(CC) $(CFLAGS) -o hello StanfordLinkedListProblem.c