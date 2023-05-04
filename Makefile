all: integral

clean: 
	rm -rf  source.o functions.o

sourse.o: source.c
	gcc -c -o source.o source.c -m32

functions.o: functions.asm
	nasm -f elf32 -o functions.o functions.asm

integral: sourse.o functions.o
	gcc -o integral source.o functions.o -m32 -lm
