all:learn

learn: learn.c
	gcc -Wall -Werror -fsanitize=address -o learn learn.c

clean:
	rm -rf learn 

