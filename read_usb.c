#include "read_usb.h"

/*
This code configures the file descriptor for use as a serial port.
*/
void configure(int fd) {
  struct termios pts;
  tcgetattr(fd, &pts);
  cfsetospeed(&pts, 9600);
  cfsetispeed(&pts, 9600);

  /*
  // You may need to un-comment these lines, depending on your platform.
  pts.c_cflag &= ~PARENB;
  pts.c_cflag &= ~CSTOPB;
  pts.c_cflag &= ~CSIZE;
  pts.c_cflag |= CS8;
  pts.c_cflag &= ~CRTSCTS;
  pts.c_cflag |= CLOCAL | CREAD;
  pts.c_iflag |= IGNPAR | IGNCR;
  pts.c_iflag &= ~(IXON | IXOFF | IXANY);
  pts.c_lflag |= ICANON;
  pts.c_oflag &= ~OPOST;
  */

  tcsetattr(fd, TCSANOW, &pts);

}

void* read_usb(void* file_ptr){
  // get the name from the command line
  filename = (char*)file_ptr;

  //Each iteration of this outer loop equates to one temperature reading
  while(1){

      //Check that the user has not selected to 'quit' in the terminal
      bool localQ;
      pthread_mutex_lock(&quit_lock);
      localQ = globalQ;
      pthread_mutex_unlock(&quit_lock);

      //If the user has selected to 'quit', return NULL which closes the thread
      if(localQ == false){
          return NULL;
      }

      //Integer to track the index of the characters being read of each temperature
      int count = 0;

      //Check that the connection is maintained
      bool localConn;
      pthread_mutex_lock(&conn_lock);
      localConn = connected;
      pthread_mutex_unlock(&conn_lock);

      //If the connection to the file descriptor has been lost, continue trying to open it 
      if(connected == false){
          
          fd = open(filename, O_RDWR | O_NOCTTY);
          if(fd > 0){
            pthread_mutex_lock(&conn_lock);
            connected = true;
            pthread_mutex_unlock(&conn_lock);
            configure(fd);
          } else {
            sleep(2);
            continue;
          }

      //If the connection is maintained
      } else {
        //This while loop operates to iterate across each character of the temperature reading
        while(1) {
            int bytesRead = read(fd, &buf, 1);
            if (bytesRead > 0) {
                temp[count++] = buf;
                if (buf == '\n')
                {
                    temp[count] = '\0';
                    
                    // if !blank then strcpy
                    if (strlen(temp) > 3) {
                        //Copy the current temperature to the global copy of the temperature
                        pthread_mutex_lock(&temp_lock);
                        strcpy(current_temp, temp);
                        pthread_mutex_unlock(&temp_lock);
                    }
                    
                    break;
                }
            } else if (bytesRead < 0){
                //If the file descriptor could not be read, the connection has been lost
                pthread_mutex_lock(&conn_lock);
                connected = false;
                pthread_mutex_unlock(&conn_lock);
                close(fd);
                printf("bytes read is less than 0\n");
                perror("error");
                sleep(5);
                break;
            }             
        }
      }
  }     
}


//make it write_usb doesnt need to be threadable
void write_usb(char c){
  if(c == 'j'){
    //Write Joel to the 7 segment display
    write(fd, "j", 1);
  }
  if(c == 'o'){
    //Change LED color to green
    write(fd, "o", 1);
  }
  if(c == 't'){
    //going toStandby, set 7 segment to blank
    write(fd, "t", 1);
  }
  if(c == 'f'){
    //coming fromStandby, wake 7 segment back up
    write(fd, "f", 1);
  }

  if(c == 'u'){
    //coming fromStandby, wake 7 segment back up
    write(fd, "u", 1);
  }
}

