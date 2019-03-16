CFLAGS= -Wall -O -fPIC -shared 
CC=mipsel-linux-gcc
#CC=gcc
#COMMAND=-L/yuan/pro/https/lib-pc -lcurl -lssl -lcrypto -lm
COMMAND=-L/yuan/pro/https/lib -lcurl -lssl -lcrypto -lz -lm 
obj +=szhy_get.o http_get.o cJSON.o
TARGET = libszhy_get.so
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o
