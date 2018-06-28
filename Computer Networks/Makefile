CC=gcc
CFLAGS=-pthread

all: serv4 serv3 serv2 serv1 client

serv4: serv4.o
	$(CC) serv4.o $(CFLAGS) -o serv4

serv4.o: serv4.c
	$(CC) -c serv4.c $(CFLAGS)

serv3: serv3.o
	$(CC) serv3.o -o serv3

serv3.o: serv3.c
	$(CC) -c serv3.c

serv2: serv2.o
	$(CC) serv2.o -o serv2

serv2.o: serv2.c
	$(CC) -c serv2.c

serv1: serv1.o
	$(CC) serv1.o -o serv1

serv1.o: serv1.c
	$(CC) -c serv1.c

client: client.o
	$(CC) client.o -o client

client.o: client.c
	$(CC) -c client.c

clean:
	rm -f *.o serv4 serv3 serv2 serv1 client

	
