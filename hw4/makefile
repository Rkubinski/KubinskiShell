mykernel: ram.o pcb.o interpreter.o shell.o kernel.o shellmemory.o cpu.o memorymanager.o DISK_driver.o IOSCHEDULER.o
	gcc -Wall -o mykernel2 ram.o pcb.o interpreter.o shell.o kernel.o shellmemory.o cpu.o memorymanager.o DISK_driver.o IOSCHEDULER.o -g
ram.o: ram.c ram.h kernel.h
	gcc -c ram.c
kernel.o: kernel.c ram.h pcb.h shell.h cpu.h kernel.h IOSCHEDULER.h DISK_driver.h shellmemory.h
	gcc -c kernel.c 
memorymanager.o: memorymanager.c memorymanager.h kernel.h 
	gcc -c memorymanager.c
pcb.o: pcb.c pcb.h
	gcc -c pcb.c
cpu.o: cpu.c cpu.h shell.h 
	gcc -c cpu.c
interpreter.o: interpreter.c shellmemory.h shell.h interpreter.h kernel.h memorymanager.h
	gcc -c  interpreter.c
shellmemory.o: shellmemory.c shellmemory.h
	gcc -c shellmemory.c
shell.o: interpreter.h shell.c shell.h
	gcc -c shell.c
DISK_driver.o: DISK_driver.c DISK_driver.h 
	gcc -c DISK_driver.c
IOSCHEDULER.o: IOSCHEDULER.c IOSCHEDULER.h pcb.h
	gcc -c IOSCHEDULER.c 
