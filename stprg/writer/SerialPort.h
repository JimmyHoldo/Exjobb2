#ifndef SERIALPORT_H_INCLUDED
#define SERIALPORT_H_INCLUDED

#include <termios.h>
#include <inttypes.h>

#include <string>

#include <sstream>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <cstddef>
#include <cstdio>
#include <stdio.h>

class SerialPort
{
    public:

        void open_port_serial(std::string  str);
        int initport();
        int getPort();
        void setPort(int i);
        void getData(char* outStr);
        void setData(char* inStr);
        int read_from_zigbee();
        int write_to_zigbee();

        void uninitialize();

        ~SerialPort();

    private:
        void append(int i, int n, char* indata);
        int serial_fd = -1;
        char data[12];

};

#endif // SERIALPORT_H_INCLUDED
