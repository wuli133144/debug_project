CC=cc
CFLAGS= -g -I../../include
LIBS= -L../.. ../../libsg_ssl.a ../../libsg_crypto.a
EXAMPLES=saccept sconnect

all: $(EXAMPLES) 

saccept: saccept.o
	$(CC) -o saccept saccept.o $(LIBS)

sconnect: sconnect.o
	$(CC) -o sconnect sconnect.o $(LIBS)

clean:	
	rm -f $(EXAMPLES) *.o

