CFLAGS=-I../../include -Wall -Werror -g

all: state_machine

state_machine: state_machine.o
	$(CC) -o state_machine state_machine.o -L../.. -lsg_ssl -lsg_crypto

test: state_machine
	./state_machine 10000 ../../apps/server.pem ../../apps/server.pem
