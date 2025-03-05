all: prog.o library.o
	gcc prog.o library.o -o prog -g

prog.o: prog.c 
	gcc -c prog.c 

library.o: library.c 
	gcc -c library.c 

clean:
	rm -f *.o
	
