CC     = gcc
CFLAGS = -lrt -lm

all: real3 real0

real3: real_time_modified.c
	$(CC) -O3 -o real3 real_time_modified.c $(CFLAGS)

real0: real_time_modified.c
	$(CC) -O0 -o real0 real_time_modified.c $(CFLAGS)

clean:
	rm -rf real0 real3
