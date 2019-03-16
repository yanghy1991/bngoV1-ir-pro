CFLAGS= -Wall -O2
#CC=mipsel-linux-gcc
CC=gcc
COMMAND=-L/yuan/pro/https/lib-pc -lcurl -lssl -lcrypto -lm
#COMMAND=-L/yuan/pro/https/lib -lcurl -lssl -lcrypto -lz -lm 
obj +=main.o http_get.o
#obj +=https_test.o
TARGET = https_test
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o curlTest
