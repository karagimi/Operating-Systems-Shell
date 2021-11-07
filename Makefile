#Michalis Karagiannakis csd4355@csd.uoc.gr

CC=gcc

FL=-o

NAME=shell

all: shell.c
	$(CC) $(FL) $(NAME) shell.c

clean:
	rm -f $(NAME)

run: shell.c
	$(CC) $(FL) $(NAME) shell.c
	./$(NAME)