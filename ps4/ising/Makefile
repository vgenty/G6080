CC=gcc
CFLAGS=-Wall
LDFLAGS=-lm
EXECUTABLE=cluster

all: $(EXECUTABLE)

$(EXECUTABLE): cluster.o random.o
	$(CC) $(CFLAGS) -o cluster cluster.o random.o $(LDFLAGS)

cluster.o:
	$(CC) $(CFLAGS) -c cluster.C

random.o:
	$(CC) $(CFLAGS) -c random.C

clean:
	rm *.o cluster
