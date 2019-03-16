CFLAGS= -Wall -O2
CC=mipsel-linux-gcc
COMMAND=-L/yuan/pro/https/lib -lcurl -lssl -lcrypto -lz
obj += cacertinmem.o
TARGET = https
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o
