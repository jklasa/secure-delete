all: secure-delete

secure-delete: secure-delete.c overwrite.c
	gcc -g -Wall -Werror -o sdel secure-delete.c overwrite.c
clean:
	rm -f *.o *.out sdel 
