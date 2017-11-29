all: sorter

sorter:  
	gcc -g -Werror -Wall -fsanitize=address -o sorter sorter.c mergesort.c -pthread
	
clean:
	rm -rf sorter mergesort.o
