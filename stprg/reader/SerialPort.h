#ifndef SERIALPORT_H_INCLUDED
#define SERIALPORT_H_INCLUDED

#include <termios.h>
#include <inttypes.h>

#include <string>

class SerialPort
{
    public:

        void open_port_serial(std::string  str);
        int initport();
        int getPort();
        void getData(char* outStr);
        void setData(char* inStr);
        int read_from_zigbee();
        int write_to_zigbee();

        void uninitialize();

        ~SerialPort();

    private:
        int serial_fd = -1;
        char data[100];

};

#endif // SERIALPORT_H_INCLUDED
