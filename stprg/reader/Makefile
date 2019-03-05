#MakeFile

all: reader

reader: r.o SerialPort.o
	g++ r.o SerialPort.o `pkg-config --libs --cflags dbus-1` -o reader

r.o: reader.cpp SerialPort.h
	g++ -c reader.cpp `pkg-config --libs --cflags dbus-1` -o r.o

SerialPort.o: SerialPort.cpp SerialPort.h
	g++ -c SerialPort.cpp

clean:
	rm *.o reader
