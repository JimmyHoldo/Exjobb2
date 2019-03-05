#MakeFile

all: reader writer datagen

reader: r.o SerialPort.o
	g++ r.o SerialPort.o `pkg-config --libs --cflags dbus-1` -o reader

writer: w.o SerialPort.o
	g++ w.o SerialPort.o `pkg-config --libs --cflags dbus-1` -o writer

datagen: d.o 
	g++ d.o `pkg-config --libs --cflags dbus-1` -o datagen

r.o: reader.cpp SerialPort.h	
	g++ -c reader.cpp `pkg-config --libs --cflags dbus-1` -o r.o

w.o: writer.cpp SerialPort.h	
	g++ -c writer.cpp `pkg-config --libs --cflags dbus-1` -o w.o

d.o: datagen.cpp 	
	g++ -c datagen.cpp `pkg-config --libs --cflags dbus-1` -o d.o

SerialPort.o: SerialPort.cpp SerialPort.h
	g++ -c SerialPort.cpp

clean:
	rm *.o writer reader datagen
