all: sorter

sorter:  
	gcc -Wall -Werror -fsanitize=address -o sorter sorter.c mergesort.c -pthread
	
clean:
	rm -rf sorter mergesort.o
