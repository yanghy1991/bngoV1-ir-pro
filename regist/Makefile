CFLAGS= -Wall -O2
CC=mipsel-linux-gcc
#CC=gcc
#COMMAND=-L/yuan/pro/https/lib-pc -lcurl -lssl -lcrypto -lm
COMMAND=-L/yuan/pro/https/lib -lcurl -lssl -lcrypto -lz -lm -lszhy_get -lbngo_aes -lbngo_info -lini
obj +=main.o cJSON.o
TARGET = bngoSetTime
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o
