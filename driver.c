
/*
 This code primarily comes from
 http://www.prasannatech.net/2008/07/socket-programming-tutorial.html
 and
 http://www.binarii.com/files/papers/c_sockets.txt
 */

#include "driver.h"

int main(int argc, char *argv[])
{
    // check the number of arguments
    if (argc != 3) {
        printf("\nUsage: %s [port_number]\n", argv[0]);
        exit(-1);
    }
    
    pthread_t ard_thread;
    pthread_t server_thread;
    pthread_t quit_thread;
    pthread_mutex_init(&temp_lock, NULL);
    pthread_mutex_init(&conn_lock, NULL);
    pthread_mutex_init(&quit_lock, NULL);
    int port_number = atoi(argv[1]);

    connected = false;
    globalQ = true;

    if (port_number <= 1024) {
        printf("\nPlease specify a port number greater than 1024\n");
        exit(-1);
    }

    void* void_port_number = (void*) &port_number;
    server_setup(void_port_number);

    char fileName[100];
    strcpy(fileName, argv[2]);

    int q = pthread_create(&ard_thread, NULL, &read_usb, &fileName);
    if(q != 0){
        printf("%s\n", "Failed to create ard_thread");
        return -1;
    }

    int r = pthread_create(&server_thread, NULL, &start_server, void_port_number);
    if(r != 0){
        printf("%s\n", "Failed to create server_thread");
        return -1;
    }

    int quit = pthread_create(&quit_thread, NULL, &waiting_for_q, NULL);
    if(quit != 0){
        printf("%s\n", "Failed to create quit_thread");
        return -1;
    }
    
    //DO WE NEED TO JOIN THREAD 3 FIRST?
    pthread_join(ard_thread, NULL);
    pthread_join(server_thread, NULL);
    pthread_join(quit_thread, NULL);

    return 0;
}
