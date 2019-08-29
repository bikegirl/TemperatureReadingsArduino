
/*
 This code primarily comes from
 http://www.prasannatech.net/2008/07/socket-programming-tutorial.html
 and
 http://www.binarii.com/files/papers/c_sockets.txt
 */

#include "start_server.h"
#include "server_setup.h"

//Waiting for the user to input q and quit the program
void* waiting_for_q(){
    char input[100];
    while(1) {    
        scanf("%s", input);
        if(strcmp(input, "q") == 0){
            pthread_mutex_lock(&quit_lock);
            globalQ = false;
            pthread_mutex_unlock(&quit_lock);
            printf("%s\n", "Quitting MyWonderground");
            break;
        }
    }
    return NULL;
}

//Accept and Handle request, creating new thread for new incoming requests once they arrive
void* start_server(void* VOID_PORT_NUMBER)
{
    int PORT_NUMBER = *(int*) VOID_PORT_NUMBER;
    int z = 0;

    while(1){
    // 4. accept: wait here until we get a connection on that port
        bool localQ;
        pthread_mutex_lock(&quit_lock);
        localQ = globalQ;
        pthread_mutex_unlock(&quit_lock);
        if(localQ == false){
            return NULL;
        }
        int sin_size = sizeof(struct sockaddr_in);
        int* fd = malloc(sizeof(int));

        *fd = accept(sock, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);

        if (*fd == -1) {
            free(fd);
            continue;
        }    
        
        pthread_t dispatch;

        pthread_create(&dispatch, NULL, &handle_request, fd);
        pthread_detach(dispatch);

    }
             
    // 8. close: close the socket
    close(sock);
    printf("Server shutting down\n");
    
    return 0;
}

//Handle Different requests and send back responses
void* handle_request(void* ptr){
    // printf("ptr = %p\n", ptr);
    int fd = *(int*) ptr;
    free(ptr);
    // printf("Server got a connection from (%s, %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        
    // buffer to read data into
    char request[1024];

    // 5. recv: read incoming message (request) into buffer
    int bytes_received = recv(fd,request,1024,0);
    // null-terminate the string
    request[bytes_received] = '\0';


    char firstline[1024];
    int c = 0;
    while(request[c] != '\n'){
        firstline[c] = request[c];
        c++;
    }

    char* token = strtok(firstline, " ");
    char request_type[1000];
    strcpy(request_type, token);
    
    //Ensure request type is GET
    //If not, do nothing
    if(strcmp(request_type, "GET") == 0){
        

        token = strtok(NULL, " ");

        char requested_resource[1000];

        strcpy(requested_resource, token);
        //Token now equals the requested resource

        if(strcmp(requested_resource, "/Response.html") == 0){
        //if(strcmp(requested_resource, "/") == 0){
            // this is the message that we'll send back
            char reply[15000];

            //COPY IN FROM FILE RESPONSE.HTML
            strcpy(reply, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");

            //Funtion to read in file take from: https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c
            char * buffer = 0;
            long length;
            FILE * f = fopen ("./Response.html", "rb");

            if (f)
            {
                fseek (f, 0, SEEK_END);
                length = ftell (f);
                fseek (f, 0, SEEK_SET);
                buffer = malloc (length);
                if (buffer)
                {
                fread (buffer, 1, length, f);
                }
                fclose (f);
            } else {
                //Error condition for file not opening
                strcpy(reply, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><p>");
                strcat(reply, "Error: Requested resource failed to open.");
                strcat(reply, "</p></html>"); 
            }

            if (buffer)
            {
                //If the file was read in, concatenate entire file onto response
                strcpy(reply, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
                strcat(reply, buffer);
                free(buffer);
            }
            
            // 6. send: send the outgoing message (response) over the socket
            // note that the second argument is a char*, and the third is the number of chars
            send(fd, reply, strlen(reply), 0);
        }

        if(strcmp(requested_resource, "/jquery.js") == 0){

            // this is the message that we'll send back
            char reply[150000];

            //COPY IN FROM FILE RESPONSE.HTML
            strcpy(reply, "HTTP/1.1 200 OK\nContent-Type: text/js\n\n");

            //Function to read in file take from: https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c
            char * buffer = 0;
            long length;
            FILE * f = fopen ("./jquery.js", "rb");

            if (f)
            {
                fseek (f, 0, SEEK_END);
                length = ftell (f);
                fseek (f, 0, SEEK_SET);
                buffer = malloc (length);
                if (buffer)
                {
                fread (buffer, 1, length, f);
                }
                
                fclose (f);
            } else {
                //Error condition for file not opening
                strcpy(reply, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><p>");
                strcat(reply, "Error: Requested resource failed to open.");
                strcat(reply, "</p></html>"); 
            }
            if (buffer)
            {
                //If the file was read in, concatenate entire file onto response
                strcat(reply, buffer);

                free(buffer);
            } else {
                //Error condition for file not reading
                strcpy(reply, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><p>");
                strcat(reply, "Error: Requested resource not read.");
                strcat(reply, "</p></html>");                        
            }
            
            // printf("Server responded:\n%s\n", reply);
            
            // 6. send: send the outgoing message (response) over the socket
            // note that the second argument is a char*, and the third is the number of chars
            send(fd, reply, strlen(reply), 0);
        }  

        if(strcmp(requested_resource, "/MyWonderground.js") == 0){

            // this is the message that we'll send back
            char reply[15000];

            //COPY IN FROM FILE RESPONSE.HTML
            strcpy(reply, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");

            //Funtion to read in file take from: https://stackoverflow.com/questions/174531/how-to-read-the-content-of-a-file-to-a-string-in-c
            char * buffer = 0;
            long length;
            FILE * f = fopen ("./MyWonderground.js", "rb");

            if (f)
            {
                fseek (f, 0, SEEK_END);
                length = ftell (f);
                fseek (f, 0, SEEK_SET);
                buffer = malloc (length);
                if (buffer)
                {
                fread (buffer, 1, length, f);
                }
                fclose (f);
            } else {
                //Error condition for file not opening
                strcpy(reply, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><p>");
                strcat(reply, "Error: Requested resource failed to open.");
                strcat(reply, "</p></html>"); 
            }

            if (buffer)
            {
                //If the file was read in, concatenate entire file onto response
                strcpy(reply, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
                strcat(reply, buffer);
                free(buffer);
            }
            
            // printf("Server responded:\n%s\n", reply);
            
            // 6. send: send the outgoing message (response) over the socket
            // note that the second argument is a char*, and the third is the number of chars
            send(fd, reply, strlen(reply), 0);
        }                             

        if(strcmp(requested_resource, "/getTemperature") == 0){
            pthread_mutex_lock(&conn_lock);
            bool localConn = connected;
            pthread_mutex_unlock(&conn_lock);

            if(localConn == false){
                char response[15000];
                sprintf(response, "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{}");
                
                printf("Server responded:\n%s\n", response);
                
                // 6. send: send the outgoing message (response) over the socket
                // note that the second argument is a char*, and the third is the number of chars
                send(fd, response, strlen(response), 0);
            } else {
                char local_temp[100];
                pthread_mutex_lock(&temp_lock);
                strcpy(local_temp, current_temp);
                local_temp[strlen(local_temp)-1] = '\0';
                pthread_mutex_unlock(&temp_lock);

                // this is the message that we'll send back
                char reply[15000];
                sprintf(reply, "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{\"a\": \"%s\" }", local_temp);
                
                // 6. send: send the outgoing message (response) over the socket
                // note that the second argument is a char*, and the third is the number of chars
                send(fd, reply, strlen(reply), 0);

            }

        }

        if(strcmp(requested_resource, "/getJoel") == 0){
            write_usb('j');
            char reply[1500];
            sprintf(reply, "HTTP/1.1 200 OK\n");
            
            // 6. send: send the outgoing message (response) over the socket
            // note that the second argument is a char*, and the third is the number of chars
            send(fd, reply, strlen(reply), 0);
        }

        if(strcmp(requested_resource, "/turnLEDon") == 0){
            write_usb('o');
            char reply[1500];
            sprintf(reply, "HTTP/1.1 200 OK\n");
            
            // 6. send: send the outgoing message (response) over the socket
            // note that the second argument is a char*, and the third is the number of chars
            send(fd, reply, strlen(reply), 0);
        }

        if(strcmp(requested_resource, "/toStandby") == 0){
            write_usb('t');
            char reply[1500];
            sprintf(reply, "HTTP/1.1 200 OK\n");
            
            // 6. send: send the outgoing message (response) over the socket
            // note that the second argument is a char*, and the third is the number of chars
            send(fd, reply, strlen(reply), 0);

        }

        if(strcmp(requested_resource, "/fromStandby") == 0){
            write_usb('f');
            char reply[1500];
            sprintf(reply, "HTTP/1.1 200 OK\n");
            
            // 6. send: send the outgoing message (response) over the socket
            // note that the second argument is a char*, and the third is the number of chars
            send(fd, reply, strlen(reply), 0);

        }

        if(strcmp(requested_resource, "/toggleUnits") == 0){
            write_usb('u');
            char reply[1500];
            sprintf(reply, "HTTP/1.1 200 OK\n");
            
            // 6. send: send the outgoing message (response) over the socket
            // note that the second argument is a char*, and the third is the number of chars
            send(fd, reply, strlen(reply), 0);

        }

    }  
    // 7. close: close the connection
    close(fd);
    return NULL;
    
}
