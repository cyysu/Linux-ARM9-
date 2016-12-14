
#include <string.h>
#include "i2c.h"

unsigned char at24cxx_read(unsigned char address)
{
	unsigned char val;
	printf("at24cxx_read address = %d\r\n", address);
    i2c_write(0xA0, &address, 1);
	printf("at24cxx_read send address ok\r\n");
    i2c_read(0xA0, (unsigned char *)&val, 1);
	printf("at24cxx_read get data ok\r\n");
	return val;
}

void at24cxx_write(unsigned char address, unsigned char data)
{
	unsigned char val[2];
	val[0] = address;
	val[1] = data;
    i2c_write(0xA0, val, 2);
}

