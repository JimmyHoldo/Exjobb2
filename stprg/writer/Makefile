#MakeFile

all: writer datagen

writer: w.o SerialPort.o
	g++ w.o SerialPort.o `pkg-config --cflags --libs glib-2.0 gio-2.0` -o writer

datagen: d.o
	g++ d.o `pkg-config --cflags --libs glib-2.0 gio-2.0` -o datagen

w.o: writer.cpp SerialPort.h
	g++ -c writer.cpp `pkg-config --cflags --libs glib-2.0 gio-2.0` -o w.o

d.o: datagen.cpp
	g++ -c datagen.cpp `pkg-config --cflags --libs glib-2.0 gio-2.0` -o d.o

SerialPort.o: SerialPort.cpp SerialPort.h
	g++ -c SerialPort.cpp

clean:
	rm *.o writer datagen
