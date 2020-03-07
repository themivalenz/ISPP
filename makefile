CFLAGS = -O
CC = g++
ISPP: main.o functions.o
	$(CC) $(CFLAGS) -o ISPP main.o functions.o
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
functions.o: functions.cpp
	$(CC) $(CFLAGS) -c functions.cpp
clean:
	rm -f core *.o 
