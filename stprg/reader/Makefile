#MakeFile

all: reader

reader: r.o SerialPort.o
	g++ r.o SerialPort.o -o reader

r.o: reader.cpp ../../cpp_serial/SerialPort.h
	g++ -c reader.cpp  -o r.o

SerialPort.o: ../../cpp_serial/SerialPort.cpp ../../cpp_serial/SerialPort.h
	g++ -c ../../cpp_serial/SerialPort.cpp

clean:
	rm *.o reader
