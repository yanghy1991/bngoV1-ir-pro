CFLAGS= -Wall -O2
CC=mips-linux-gnu-gcc
deps= bngo_ir.h
obj += main.o 
TARGET = bngo_ir_test
tupe: $(obj)
		$(CC) $(CFLAGS) -o $(TARGET) $(obj)  -L/yuan/pro/ko/yaokan -l_yaokan_ir

%.o: %.c $(deps)
		$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(obj)
