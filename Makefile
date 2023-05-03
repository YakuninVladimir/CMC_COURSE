all: integral

clean: 
	rm -rf integral sourse.o functions.o

sourse.o: sourse.c
	gcc -c -o sourse.o sourse.c -m32

functions.o: functions.asm
	nasm -f elf32 -o functions.o functions.asm

integral: sourse.o functions.o
	gcc -o integral sourse.o functions.o -m32 -lm
