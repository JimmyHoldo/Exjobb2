#include <iostream>
#include <dbus/dbus.h>
#include <unistd.h>
#include <time.h>

using namespace std;

int main()
{
    struct timespec ts;
    ts.tv_sec  = 0;
    ts.tv_nsec = 1000; //250000000L;

    DBusError error;
    dbus_error_init(&error);

    DBusConnection *connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (!connection || dbus_error_is_set(&error)) {
        perror("Connection error.");
        exit(1);
    }

    const int ret = dbus_bus_request_name(connection, "test.foo.caller2", DBUS_NAME_FLAG_ALLOW_REPLACEMENT, &error);
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER || dbus_error_is_set(&error)) {
        perror("Ouch.");
        exit(1);
    }

    int counter = 0;
    while(1){


        DBusMessage *const msg = dbus_message_new_method_call("test.foo.bar",
        "/test/foo/Object",
        "test.foo.Roll",
        "Method");

        std::string text = "1: ";
        text += std::to_string(counter % 1000);
        counter++;
        const char *str = text.c_str();
        std::cout << str << std::endl;
        dbus_message_append_args(msg, DBUS_TYPE_STRING, &str, DBUS_TYPE_INVALID);
        if (!msg) {
            perror("Ouch.");
            exit(1);
        }

        dbus_uint32_t serial = dbus_message_get_serial(msg);
        dbus_connection_send(connection, msg, &serial);
        dbus_connection_flush(connection);
        dbus_message_unref(msg);
        nanosleep(&ts, NULL);
    }

    dbus_connection_unref(connection);

    cout << "Hello world!" << endl;
    return 0;
}
