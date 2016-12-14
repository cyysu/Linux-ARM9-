#include <stdio.h>
#include "serial.h"
#include "i2c.h"

unsigned char at24cxx_read(unsigned char address);
void at24cxx_write(unsigned char address, unsigned char data);


int main()
{
    char c;
    char str[200];
    int i;
	int address;
	int data;
    
    uart0_init();   // 波特率115200，8N1(8个数据位，无校验位，1个停止位)
    
    i2c_init();
    
    while (1)
    {
        printf("\r\n##### AT24CXX Menu #####\r\n");
        printf("[R] Read AT24CXX\n\r");
        printf("[W] Write AT24CXX\n\r");
        printf("Enter your selection: ");

        c = getc();
        printf("%c\n\r", c);
        switch (c)
        {
            case 'r':
            case 'R':
            {
                printf("Enter address: ");
                i = 0;
                do
                {
                    c = getc();
                    str[i++] = c;
                    putc(c);
                } while(c != '\n' && c != '\r');
                str[i] = '\0';

                while(--i >= 0)
                {
                    if (str[i] < '0' || str[i] > '9')
                        str[i] = ' ';
                }

                sscanf(str, "%d", &address);
				printf("\r\nread address = %d\r\n", address);
				data = at24cxx_read(address);
				printf("data = %d\r\n", data);
                    
                break;
            }
            
            case 'w':
            case 'W':
            {
                printf("Enter address: ");
                i = 0;
                do
                {
                    c = getc();
                    str[i++] = c;
                    putc(c);
                } while(c != '\n' && c != '\r');
                str[i] = '\0';
				printf("\r\n");

                while(--i >= 0)
                {
                    if (str[i] < '0' || str[i] > '9')
                        str[i] = ' ';
                }

                sscanf(str, "%d", &address);
				//printf("get str %s\r\n", str);

                printf("Enter data: ");
                i = 0;
                do
                {
                    c = getc();
                    str[i++] = c;
                    putc(c);
                } while(c != '\n' && c != '\r');
                str[i] = '\0';
				printf("\r\n");
				//printf("get str %s\r\n", str);

                while(--i >= 0)
                {
                    if (str[i] < '0' || str[i] > '9')
                        str[i] = ' ';
                }

                sscanf(str, "%d", &data);
				//address = 12;
				//data = 13;
				printf("write address %d with data %d\r\n", address, data);
				
				at24cxx_write(address, data);

                break;
            }
        }
        
    }
    
    return 0;
}
