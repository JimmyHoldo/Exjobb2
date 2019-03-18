#MakeFile

all: reader writer datagen

reader: r.o SerialPort.o
	g++ bin/r.o bin/SerialPort.o `pkg-config --cflags --libs glib-2.0 gio-2.0` -o reader

writer: w.o SerialPort.o
	g++ bin/w.o bin/SerialPort.o `pkg-config --cflags --libs glib-2.0 gio-2.0` -o writer

datagen: d.o
	g++ bin/d.o `pkg-config --cflags --libs glib-2.0 gio-2.0` -o datagen

r.o: reader.cpp ../cpp_serial/SerialPort.h
	g++ -c reader.cpp `pkg-config --cflags --libs glib-2.0 gio-2.0` -o bin/r.o

w.o: writer.cpp ../cpp_serial/SerialPort.h
	g++ -c writer.cpp `pkg-config --cflags --libs glib-2.0 gio-2.0` -o bin/w.o

d.o: datagen.cpp
	g++ -c datagen.cpp `pkg-config --cflags --libs glib-2.0 gio-2.0` -o bin/d.o

SerialPort.o: ../cpp_serial/SerialPort.cpp ../cpp_serial/SerialPort.h
	g++ -c ../cpp_serial/SerialPort.cpp -o bin/SerialPort.o

clean:
	rm bin/*.o writer reader datagen
