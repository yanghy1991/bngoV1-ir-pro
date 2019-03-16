CFLAGS= -Wall -O2
CC=mipsel-linux-gcc
#CC=gcc
obj +=main.o city_sohu.o
TARGET = socket_time
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o socket_time
