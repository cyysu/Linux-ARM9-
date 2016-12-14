CROSS=arm-linux-

all: button_test

button_test: button_test.c
	$(CROSS)gcc -o $@ button_test.c -static

clean:
	@rm -rf button_test *.o
