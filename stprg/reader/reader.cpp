#include <iostream>
#include "../../cpp_serial/SerialPort.h"

#include <string.h>
#include <unistd.h>
#include <cstddef>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    SerialPort port;

   while(port.getPort() == -1){

       port.open_port_serial("/dev/ttyUSB0");

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
            char dataStr[11];
            port.getData(dataStr);

            std::cout << dataStr << std::endl;
            char filename[50];
            sprintf(filename, "../../files/test/arm_%s.txt", argv[1]);
            // FILE *f = fopen(filename, "a");
            // if (f == NULL)
            // {
            //     printf("Error opening file!\n");
            //     exit(1);
            // }
            //
            //
            // fprintf(f, "%s\n", dataStr);
            //fclose(f);
        }
    }

    std::cout << "Hello world!" << std::endl;
    return 0;
}
