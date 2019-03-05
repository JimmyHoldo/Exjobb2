#include <iostream>
#include "SerialPort.h"

#include <string.h>
#include <unistd.h>
#include <cstddef>
#include <cstdio>
#include <dbus/dbus.h>



int main()
{
    SerialPort port;

    while(port.getPort() == -1){

        port.open_port_serial("/dev/ttyUSB1");

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
    //port.setPort(3);
    DBusError error;
    dbus_error_init(&error);

    DBusConnection *connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (!connection || dbus_error_is_set(&error)) {
        perror("Connection error.");
        exit(1);
    }

    const int ret = dbus_bus_request_name(connection, "test.foo.caller", DBUS_NAME_FLAG_ALLOW_REPLACEMENT, &error);
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER || dbus_error_is_set(&error)) {
        perror("Ouch.");
        exit(1);
    }

    while(1){

        if(port.read_from_zigbee() > 0)
        {
            DBusMessage *const msg = dbus_message_new_method_call("test.foo.bar",
                                                                "/test/foo/Object",
                                                                "test.foo.Roll",
                                                                "Method");

            char dataStr[100];
            port.getData(dataStr);
            const char *str = dataStr;
            dbus_message_append_args(msg, DBUS_TYPE_STRING, &str, DBUS_TYPE_INVALID);
            if (!msg) {
                perror("Ouch.");
                exit(1);
            }

            dbus_uint32_t serial = dbus_message_get_serial(msg);
            dbus_connection_send(connection, msg, &serial);
            dbus_connection_flush(connection);
            dbus_message_unref(msg);
        }
    }

    dbus_connection_unref(connection);

    return 0;
}
