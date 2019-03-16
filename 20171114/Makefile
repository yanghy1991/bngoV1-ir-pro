CFLAGS= -Wall -O2
CC=mipsel-linux-gcc
COMMAND=-L/yuan/pro/https/lib -lcurl -lssl -lcrypto -lz -lm -lir_control -l_yaokan_ir 
obj +=main.o
TARGET = bngo_ir
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o
