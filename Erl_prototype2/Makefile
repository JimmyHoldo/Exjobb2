#Makefile

all: readerwriterprg

readerwriterprg: serial_funcs.c erl_comm.c port.c
			gcc -o readerwriterprg serial_funcs.c erl_comm.c port.c

clean:
	rm *.o readerwriterprg
