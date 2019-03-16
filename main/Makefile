CFLAGS= -Wall -O2
#CFLAGS=  -O -fPIC -shared 
CC=mipsel-linux-gcc
COMMAND=-L/yuan/pro/https/lib -lcurl -lssl -lcrypto -lz -lm -l_yaokan_ir
#COMMAND=-L/yuan/pro/https/lib -lcurl -lssl -lcrypto -lz -lm 
obj +=main.o cacertinmem.o https_interface.o https_result.o cJSON.o 
TARGET = https
#TARGET = libir_control.so
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o
