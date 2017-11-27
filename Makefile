all: sorter

sorter:  
	gcc -Wall -Werror -fsanitize=address -o sorter sorter.c mergesort.c
	
clean:
	rm -rf sorter mergesort.o
