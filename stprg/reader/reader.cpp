#include <iostream>
#include "SerialPort.h"

#include <string.h>
#include <unistd.h>
#include <cstddef>
#include <cstdio>
#include <dbus/dbus.h>
#include <stdio.h>


int main()
{
    SerialPort port;

   while(port.getPort() == -1){

       port.open_port_serial("/dev/ttyUSB2");

       if (port.getPort() == -1)
               printf("Error opening serial port /dev/ttyUSB2 \n");
           else
           {
               printf("Serial Port /dev/ttyUSB2 is Open\n");
               if (port.initport() == -1)
               {
                   printf("Error Initializing port");
                   port.uninitialize();
                   return 0;
               }
       }
   }
   //std::cout << port.getPort() << std::endl;


    while(1){

        if(port.read_from_zigbee() > 0)
        {
            FILE *f = fopen("test.txt", "w");
            if (f == NULL)
            {
                printf("Error opening file!\n");
                exit(1);
            }
            char dataStr[100];
            port.getData(dataStr);

            fprintf(f, "%s\n", dataStr);
            //std::cout << dataStr << std::endl;
            fclose(f);
        }
    }

    std::cout << "Hello world!" << std::endl;
    return 0;
}
