mysh: ram.o pcb.o interpreter.o shell.o kernel.o shellmemory.o cpu.o
	gcc -o mykernel ram.o pcb.o interpreter.o shell.o kernel.o shellmemory.o cpu.o -g
ram.o: ram.c ram.h
	gcc -c ram.c
kernel.o: kernel.c ram.h pcb.h shell.h cpu.h kernel.h
	gcc -c kernel.c 
pcb.o: pcb.c pcb.h
	gcc -c pcb.c
cpu.o: cpu.c cpu.h shell.h
	gcc -c cpu.c
interpreter.o: interpreter.c shellmemory.h shell.h interpreter.h kernel.h
	gcc -c  interpreter.c
shellmemory.o: shellmemory.c shellmemory.h
	gcc -c shellmemory.c
shell.o: interpreter.h shell.c shell.h
	gcc -c shell.c
