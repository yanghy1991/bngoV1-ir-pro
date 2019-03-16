CFLAGS=-O -Wall -fPIC -shared 
CC=mipsel-linux-gcc
obj +=bngo_info.o cJSON.o 
COMMAND=-L/yuan/pro/https/lib  -lbngo_aes -lini
TARGET = libbngo_info.so
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o
