CROSS=arm-linux-

all: regeditor

regeditor: regeditor.c
	$(CROSS)gcc -o $@ regeditor.c -static

clean:
	@rm -rf regeditor *.o
