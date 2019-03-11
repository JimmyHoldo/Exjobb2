#include <iostream>
#include <glib.h>
#include <gio/gio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

using namespace std;

int main()
{
    struct timespec ts;
    ts.tv_sec  = 0;
    ts.tv_nsec = 200000000; //250000000L;

    int counter = 0;
    while(1){

        std::string text = "2 Msg: ";
        text += std::to_string((counter % 899)+100);
        counter++;
        const char *str = text.c_str();
        std::cout << str << std::endl;

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
        GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
        GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
        g_output_stream_write  (ostream,
                                str, /* your message goes here */
                                strlen(str), /* length of your message */
                                NULL,
                                &error);
        /* don't forget to check for errors */
        if (error != NULL)
        {
            g_error (error->message);
        }

        nanosleep(&ts, NULL);
    }

    cout << "Hello world!" << endl;
    return 0;
}
