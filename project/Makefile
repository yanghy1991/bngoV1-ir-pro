CFLAGS= -Wall -O2
#CC=mipsel-linux-gcc
CC=gcc
COMMAND=-L/yuan/pro/https/lib-pc -lssl -lcrypto
obj += main.o https_interface.o
TARGET = md5_endcode
https: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o
