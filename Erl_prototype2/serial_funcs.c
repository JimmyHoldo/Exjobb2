#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h>
#include <termios.h> /* POSIX terminal control definitions */
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

//Initialize serial port
int initport(int fd)
{
    int portstatus = 0;

    struct termios options;

    // Get the current options for the port...
    tcgetattr(fd, &options);

    // Set the baud rates to 9600...
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    // Enable the receiver and set local mode...
    options.c_cflag |= (CLOCAL | CREAD);

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    //options.c_cflag |= SerialDataBitsInterp(8);     /* CS8 - Selects 8 data bits */
    options.c_cflag &= ~CRTSCTS;                      // Disable hardware flow control
    options.c_iflag &= ~(IXON | IXOFF | IXANY);       // Disable XON XOFF (for transmit and receive)
    //options.c_cflag |= CRTSCTS;                     /* Enable hardware flow control */

    options.c_cc[VMIN] = 1;   //Minimum characters to be read
    options.c_cc[VTIME] = 100;    //Time to wait for data (tenths of seconds)
    options.c_oflag &=~OPOST;
    options.c_iflag &=~(ICANON | ECHO | ECHOE | ISIG);
    // Set the new options for the port...
    tcsetattr(fd, TCSANOW, &options);

    //Set the new options for the port...
    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &options) == -1)
    {
        perror("On tcsetattr:");
        portstatus = -1;
    }
    else
        portstatus = 1;

    return (portstatus);
}

int open_port_serial(char *str)
{
    int fd; /* File descriptor for the port */
    fd = open(str, O_RDWR | O_NOCTTY | O_NONBLOCK);

    return fd;
}

void append(int i, int n, char* content, char* indata)
{
        for(int j=0; j<n; j++){
            content[i+j] = indata[j];
        }
}

int read_from_zigbee(int serial_fd, char *content){
    struct timespec ts;
    ts.tv_sec  = 0;
    ts.tv_nsec = 100000000;

    int n1 = 0;
    char indata[11];
    while(n1 < 1 || (n1 != 10)){
        int n = read(serial_fd, indata, 10-n1);
        if(n == 10){
            strncpy(content, indata, 10);
            return n;
        } else if(n != -1){
            append(n1, n, content, indata);
            n1 = n1+n;
        }
        if(n1 < 0 || (n1 != 10)) {
            nanosleep(&ts, NULL);
        }
    }
    return n1;
}

int write_to_zigbee(int serial_fd, char *content)
{
    int n = write(serial_fd, content, strlen(content));
    if (n < 0)
    {
      printf("write() of %ld bytes failed!\n",sizeof(*content));
      return 0;
    }
    return 1;
}
