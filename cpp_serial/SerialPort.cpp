#include "SerialPort.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

using namespace std;


void SerialPort::open_port_serial(std::string  str)
{
    int fd; /* File descriptor for the port */
    fd = open(str.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    serial_fd = fd;
}

//Initialize serial port
int SerialPort::initport()
{
    int portstatus = 0;

    struct termios options;

    // Get the current options for the port...
    tcgetattr(serial_fd, &options);

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
    options.c_cc[VTIME] = 2;    //Time to wait for data (tenths of seconds)
    options.c_oflag &=~OPOST;
    options.c_iflag &=~(ICANON | ECHO | ECHOE | ISIG);
    // Set the new options for the port...
    tcsetattr(serial_fd, TCSANOW, &options);

    //Set the new options for the port...
    tcflush(serial_fd, TCIFLUSH);
    if (tcsetattr(serial_fd, TCSANOW, &options) == -1)
    {
        perror("On tcsetattr:");
        portstatus = -1;
    }
    else
        portstatus = 1;

    return (portstatus);
}

int SerialPort::setBlocking(int should_block)
{
    struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (serial_fd, &tty) != 0)
        {
            printf("error %d from tggetattr\n", errno);
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 10;            // 1 seconds read timeout

        if (tcsetattr (serial_fd, TCSANOW, &tty) != 0)
                printf("error %d setting term attributes\n", errno);
}

int SerialPort::getPort()
{
    return serial_fd;
}

void SerialPort::getData(char* outStr)
{
  for(int i=0; i < 11; ++i){
    outStr[i] = data[i];
  }
}

void SerialPort::setData(char* inStr)
{
    strncpy(data,inStr,10);
}

SerialPort::~SerialPort()
{
    uninitialize();
}

void SerialPort::uninitialize()
{
    if (serial_fd >= 0)
    {
        close(serial_fd);
        serial_fd = -1;
    }
}

int SerialPort::read_from_zigbee()
{
    struct timespec ts;
    ts.tv_sec  = 0;
    ts.tv_nsec = 95*1000*1000;

    int n1 = 0;
    char indata[11];
    while(n1 < 1 || (n1 != 10)){
        setBlocking(1);
        int n = read(serial_fd, indata, 10-n1);
        //std::cout << n << std::endl;
        setBlocking(0);
        if(n == 10){
            strncpy(data, indata, 10);
            return n;
        } else if(n != -1){
            append(n1, n, indata);
            n1 = n1+n;
        }
        if(n1 < 1 || (n1 != 10)) {
            nanosleep(&ts, NULL);
        }
    }
    return n1;
}

void SerialPort::append(int i, int n, char* indata)
{
        for(int j=0; j<n; j++){
            data[i+j] = indata[j];
        }
}

int SerialPort::write_to_zigbee()
{
    return write(serial_fd, &data, 10);
}
