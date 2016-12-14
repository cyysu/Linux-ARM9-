CFLAGS 	:= -Wall -Wstrict-prototypes -g -fomit-frame-pointer -ffreestanding
all : crt0.S  leds.c
	arm-linux-gcc $(CFLAGS) -c -o crt0.o crt0.S
	arm-linux-gcc $(CFLAGS) -c -o leds.o leds.c
	arm-linux-ld -Tleds.lds  crt0.o leds.o -o leds_elf
	arm-linux-objcopy -O binary -S leds_elf leds.bin
	arm-linux-objdump -D -m arm  leds_elf > leds.dis
clean:
	rm -f   leds.dis leds.bin leds_elf *.o
