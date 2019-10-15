CC=gcc
CFLAGS=-Wall -g3

Scheduler: scheduler-exec.o scheduler-impl.o lab5_linkedlist.o lab5_queue.o
		$(CC) $(CFLAGS) -o Scheduler scheduler-impl.o scheduler-exec.o lab5_linkedlist.o lab5_queue.o

scheduler-exec.o: scheduler-exec.c
		$(CC) $(CFLAGS) -c -o scheduler-exec.o scheduler-exec.c

scheduler-impl.o: scheduler-impl.c
		$(CC) $(CFLAGS) -c -o scheduler-impl.o scheduler-impl.c

queue.o: lab5_queue.c
		$(CC) $(CFLAGS) -c -o lab5_queue.o lab5_queue.c

linkedlist.o: lab5_linkedlist.c
		$(CC) $(CFLAGS) -c -o lab5_linkedlist.o lab5_linkedlist.c

clean:
		rm *.o 
