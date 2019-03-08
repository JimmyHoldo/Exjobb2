#include <iostream>
#include "SerialPort.h"
#include <unistd.h>
#include <dbus/dbus.h>
#include <string.h>

using namespace std;
SerialPort port;

int main()
{


    while(port.getPort() == -1){

        port.open_port_serial("/dev/ttyUSB0");

        if (port.getPort() == -1)
        printf("Error opening serial port /dev/ttyUSB1 \n");
        else
        {
            printf("Serial Port /dev/ttyUSB1 is Open\n");
            if (port.initport() == -1)
            {
                printf("Error Initializing port");
                port.uninitialize();
                return 0;
            }
        }
    }
    //std::cout << port.getPort() << std::endl;
    DBusError dbus_error;
    DBusMessage * dbus_msg = nullptr;
    DBusMessage * dbus_reply = nullptr;
    const char * dbus_result = nullptr;

    // Initialize D-Bus error
    dbus_error_init(&dbus_error);
    // Connect to D-Bus
    DBusConnection *connection = dbus_bus_get(DBUS_BUS_SESSION, &dbus_error);
    if (!connection || dbus_error_is_set(&dbus_error)) {
        perror("Connection error.");
        exit(1);
    }

    const int ret = dbus_bus_request_name(connection, "test.foo.bar", DBUS_NAME_FLAG_REPLACE_EXISTING, &dbus_error);
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER || dbus_error_is_set(&dbus_error)) {
        perror("Ouch.");
        exit(1);
    }

    while (true) {
        dbus_connection_read_write(connection, 2000);
        DBusMessage *const msg = dbus_connection_pop_message(connection);
        if (!msg) {
            std::cout << "No msg" << std::endl;
            continue;
        }
        if (dbus_message_is_method_call(msg, "test.foo.Roll", "Method")) {

            const char *dbData = NULL;
            dbus_message_get_args(msg, &dbus_error, DBUS_TYPE_STRING, &dbData, DBUS_TYPE_INVALID);
            char dataStr[20];
            // Work with the results of the remote procedure call
            port.setData((char*)dbData);
            port.getData(dataStr);
            std::cout << "Writer: " << dataStr << std::endl;

            int n = port.write_to_zigbee();
            if(n == -1){
                std::cout << "Could not write" << std::endl;
            }
        }
        dbus_message_unref(msg);
    }

    return 0;
}
