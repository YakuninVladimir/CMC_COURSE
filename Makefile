
all: integral

source.o : source.c
	gcc -m32 -c -o source.o source.c 
functions.o : functions.asm
	nasm -f elf32 -o functions.o functions.asm
integral: source.o functions.o
	gcc -o integral source.o functions.o -m32 -lm 
	
clean :
	rm -rf source.o functions.o 