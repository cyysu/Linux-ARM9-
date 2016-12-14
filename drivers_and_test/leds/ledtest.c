
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

/*
  *  ledtest <dev> <on|off>
  */

void print_usage(char *file)
{
    printf("Usage:\n");
    printf("%s <dev> <on|off>\n",file);
    printf("eg. \n");
    printf("%s /dev/leds on\n", file);
    printf("%s /dev/leds off\n", file);
    printf("%s /dev/led1 on\n", file);
    printf("%s /dev/led1 off\n", file);
}

int main(int argc, char **argv)
{
    int fd;
    char* filename;
    char val;

    if (argc != 3)
    {
        print_usage(argv[0]);
        return 0;
    }

    filename = argv[1];

    fd = open(filename, O_RDWR);
    if (fd < 0)
    {
        printf("error, can't open %s\n", filename);
        return 0;
    }

    if (!strcmp("on", argv[2]))
    {
        // ÁÁµÆ
        val = 0;
        write(fd, &val, 1);
    }
    else if (!strcmp("off", argv[2]))
    {
        // ÃðµÆ
        val = 1;
        write(fd, &val, 1);
    }
    else
    {
        print_usage(argv[0]);
        return 0;
    }
    
    
    return 0;
}

