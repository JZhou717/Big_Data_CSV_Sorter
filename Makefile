all: sorter

sorter:  
	gcc -Werror -fsanitize=address -o sorter sorter.c mergesort.c -pthread
	
clean:
	rm -rf sorter mergesort.o
