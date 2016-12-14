sdram.bin : head.S  leds.c
	arm-linux-gcc  -c -o head.o head.S
	arm-linux-gcc -c -o leds.o leds.c
	arm-linux-ld -Ttext 0x30000000 head.o leds.o -o sdram_elf
	arm-linux-objcopy -O binary -S sdram_elf sdram.bin
	arm-linux-objdump -D -m arm  sdram_elf > sdram.dis
clean:
	rm -f   sdram.dis sdram.bin sdram_elf *.o
