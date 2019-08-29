CC = clang

ARGS = -g

all: read_usb server_setup start_server driver

read_usb: read_usb.c
	$(CC) -c $(ARGS) read_usb.c

server_setup: server_setup.c
	$(CC) -c $(ARGS) server_setup.c

start_server: start_server.c read_usb.c
	$(CC) -c $(ARGS) start_server.c

driver: driver.c read_usb start_server server_setup
	 $(CC) -lpthread -o  driver $(ARGS) driver.c read_usb.o start_server.o server_setup.o

clean: 
	rm -rf *.o* *.lc4* *.dSYM*
