CFLAGS= -Wall -O2
#CFLAGS=  -O -fPIC -shared 
CC=gcc
COMMAND=-L/yuan/pro/https/lib-pc -lcurl -lssl -lcrypto -lm
obj +=main.o cacertinmem.o https_interface.o https_result.o cJSON.o 
TARGET = https
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o
