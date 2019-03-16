CFLAGS= -Wall -O2
CC=mipsel-linux-gcc
COMMAND=-L/yuan/pro/https/lib -lcurl -lssl -lcrypto -lz -lm 
obj +=heyi.o cacertinmem.o
TARGET = test
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o
