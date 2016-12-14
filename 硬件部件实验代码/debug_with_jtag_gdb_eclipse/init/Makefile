objs := init.o

all: $(objs)
	arm-elf-ld -Ttext 0x0000000 -o init_elf $^
	arm-elf-objcopy -O binary -S init_elf init.bin
	arm-elf-objdump -D -m arm init_elf > init.dis
	
%.o:%.c
	arm-elf-gcc -Wall -g -c -o $@ $<

%.o:%.S
	arm-elf-gcc -Wall -g -c -o $@ $<

clean:
	rm -f init.bin init_elf init.dis *.o		
	