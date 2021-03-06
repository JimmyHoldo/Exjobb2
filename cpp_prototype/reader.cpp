#include <iostream>
#include "../cpp_serial/SerialPort.h"

#include <string.h>
#include <unistd.h>
#include <cstddef>
#include <cstdio>
#include <glib.h>
#include <gio/gio.h>

int main()
{
    SerialPort port;

   while(port.getPort() == -1){

       port.open_port_serial("/dev/ttyUSB0");

       if (port.getPort() == -1){
           printf("Error opening serial port /dev/ttyUSB2 \n");
       }
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

    while(1){

        if(port.read_from_zigbee() > 0)
        {
            char dataStr[11];
            port.getData(dataStr);
            const char *str = (const char*)dataStr;
            std::cout  << str << std::endl;

            GError * error = NULL; /* initialize glib */

            /* create a new connection */
            GSocketConnection * connection = NULL;
            GSocketClient * client = g_socket_client_new();

            /* connect to the host */
            connection = g_socket_client_connect_to_host (client,
                                                         (gchar*)"localhost",
                                                          1500, /* your port goes here */
                                                          NULL,
                                                          &error);
             /* use the connection */
             //GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
             GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
             g_output_stream_write  (ostream,
                                     str, /* your message goes here */
                                     strlen(str), /* length of your message */
                                     NULL,
                                     &error);

             g_io_stream_close((GIOStream*)connection, NULL, NULL);
             /* don't forget to check for errors */
             if (error != NULL)
             {
                 g_error (error->message);
             }
        }
    }
    return 0;
}
