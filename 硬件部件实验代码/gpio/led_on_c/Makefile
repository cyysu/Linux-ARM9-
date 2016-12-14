led_on_c.bin : crt0.S  led_on_c.c
	arm-linux-gcc -g -c -o crt0.o crt0.S
	arm-linux-gcc -g -c -o led_on_c.o led_on_c.c
	arm-linux-ld -Ttext 0x0000000 -g  crt0.o led_on_c.o -o led_on_c_elf
	arm-linux-objcopy -O binary -S led_on_c_elf led_on_c.bin
	arm-linux-objdump -D -m arm  led_on_c_elf > led_on_c.dis
clean:
	rm -f led_on_c.dis led_on_c.bin led_on_c_elf *.o
