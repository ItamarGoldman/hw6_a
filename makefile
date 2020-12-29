
#file: makefile


CC     = gcc
CFLAGS = -Wall -g -std=c99
CCLINK = gcc
OBJS   = ransom_updated.o
EXEC   = prog.exe
RM     = rm -fr *.o *.exe


$(EXEC): $(OBJS)
	$(CCLINK) $(OBJS) -o $(EXEC)

main.o: ransom_updated.c
	$(CC) -c $(CFLAGS) -o ransom_updated.o ransom_updated.c


clean:
	$(RM)