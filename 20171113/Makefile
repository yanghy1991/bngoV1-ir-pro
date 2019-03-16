CFLAGS= -Wall -O2
CC=mipsel-linux-gcc
COMMAND=-L/yuan/pro/https/lib -lcurl -lssl -lcrypto -lz -l_yaokan_ir -lm
obj += main.o cacertinmem.o https_interface.o https_result.o cJSON.o
TARGET = https
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o
