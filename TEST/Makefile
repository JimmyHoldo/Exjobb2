all: client server

client:
	g++ client.cpp `pkg-config --cflags --libs glib-2.0 gio-2.0` -o client
server:
	g++ main.cpp `pkg-config --cflags --libs glib-2.0 gio-2.0` -o server
