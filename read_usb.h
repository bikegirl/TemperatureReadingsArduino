#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h> 

/*
This code configures the file descriptor for use as a serial port.
*/
void configure(int fd);
void* read_usb(void* fd);
void arduino_setup(char* filename);
void write_usb(char c);
int fd;
char* filename;
char buf;
int bytes_read;
char temp[100];
bool connected;

char current_temp[100];
pthread_mutex_t temp_lock;
pthread_mutex_t quit_lock;
pthread_mutex_t conn_lock;
bool globalQ;
