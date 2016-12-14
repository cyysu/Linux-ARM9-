#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

#define KERRW_IOC_READ      0
#define KERRW_IOC_WRITE     1

#define KERRW_IOC_READ16      10
#define KERRW_IOC_WRITE16     11

#define KERRW_IOC_READ8      20
#define KERRW_IOC_WRITE8     21

#define KERRW_IOC_DIRECTREAD8   70
#define KERRW_IOC_DIRECTWRITE8  71

#define KERRW_IOC_READ_CP0  2
#define KERRW_IOC_WRITE_CP0 3

#define KERRW_IOC_READ_PCICONF_8    30
#define KERRW_IOC_WRITE_PCICONF_8   31
#define KERRW_IOC_READ_PCICONF_16   40
#define KERRW_IOC_WRITE_PCICONF_16  41
#define KERRW_IOC_READ_PCICONF_32   50
#define KERRW_IOC_WRITE_PCICONF_32  51

#define KERRW_IOC_READ_PCIBASE      60
#define KERRW_IOC_WRITE_PCIBASE     61


void PrintUsage(char * name)
{
    printf("Usage:\n");
    printf("01. %s r32 <address> [num]              ---- read the val(u32) from kernel address\n", name);
    printf("02. %s w32 <address> <value> [num]      ---- write the val(u32) to kernel address\n", name);
    printf("03. %s r16 <address> [num]              ---- read the val(u16) from kernel address\n", name);
    printf("04. %s w16 <address> <value> [num]      ---- write the val(u16) to kernel address\n", name);
    printf("05. %s r8 <address> [num]               ---- read the val(u8) from kernel address\n", name);
    printf("06. %s w8 <address> <value> [num]       ---- write the val(u8) to kernel address\n", name);
    printf("07. %s dr8 <address> [num]               ---- read the val(u8) from address\n", name);
    printf("08. %s dw8 <address> <value> [num]       ---- write the val(u8) to address\n", name);
    printf("09. %s v                                ---- see the version\n", name);
}
int main(int argc, char* argv[])
{
    int fd;
    int i;
    int ret;
    unsigned int adwParams[20];

    int width;    
    int num = 1;

    if ((argc == 2) && (strcmp(argv[1], "v") == 0))
    {
        printf("%s, regeditor V1.00\n", argv[0]);
        printf("Compile date and time: %s %s\n", argv[0], __DATE__, __TIME__ );
        return 0;
    }

    fd = open("/dev/ker_r_w", O_RDWR);
    if (fd < 0)
    {
        fd = open("/dev/ker_rw", O_RDWR);
        if (fd < 0)
        {
        printf("can't open /dev/ker_r_w: %d\n", fd);
            return 0;
    }
    }

    if (argc < 2)
    {
        goto err;
    }

    for (i = 0; i < 3; i++)
    {
        adwParams[i] = ~0UL;
    }
    
    for (i = 2; i < argc; i++)
    {
        adwParams[i - 2] = strtoul(argv[i], 0, 0);
    }

    if (!strcmp(argv[1], "r32"))
    {
        if (adwParams[0] == ~0UL)
        {
            goto err;
        }

        if (adwParams[1] == ~0UL)
        {
            adwParams[1] = 1;
        }
        adwParams[2] = adwParams[1];

        for (i = 0; i < adwParams[2]; i++)
        {
            if ((ret = ioctl(fd, KERRW_IOC_READ, adwParams)))
            {
                printf("Error, can't Read Reg: 0x%x, ret: %d\n", adwParams[0], ret);
                return 0;
            }
            else
            {
                printf("Reg: 0x%08x, Value: 0x%08x\n", adwParams[0], adwParams[1]);
                adwParams[0] += 4;
            }
        }
    }
    else if (!strcmp(argv[1], "w32"))
    {
        if (adwParams[0] == ~0UL)
        {
            goto err;
        }

        if (adwParams[1] == ~0UL)
        {
            goto err;
        }
        
        if (adwParams[2] == ~0UL)
        {
            adwParams[2] = 1;
        }

        for (i = 0; i < adwParams[2]; i++)
        {
            if ((ret = ioctl(fd, KERRW_IOC_WRITE, adwParams)))
            {
                printf("Error, can't Write Reg: 0x%x, ret: %d\n", adwParams[0], ret);
                return 0;
            }
            else
            {
                adwParams[0] += 4;
            }
        }
    }
    else if (!strcmp(argv[1], "r16"))
    {
        if (adwParams[0] == ~0UL)
        {
            goto err;
        }

        if (adwParams[1] == ~0UL)
        {
            adwParams[1] = 1;
        }
        adwParams[2] = adwParams[1];

        for (i = 0; i < adwParams[2]; i++)
        {
            if ((ret = ioctl(fd, KERRW_IOC_READ16, adwParams)))
            {
                printf("Error, can't Read Reg: 0x%x, ret: %d\n", adwParams[0], ret);
                return 0;
            }
            else
            {
                printf("Reg: 0x%08x, Value: 0x%08x\n", adwParams[0], adwParams[1]);
                adwParams[0] += 4;
            }
        }
    }
    else if (!strcmp(argv[1], "w16"))
    {
        if (adwParams[0] == ~0UL)
        {
            goto err;
        }

        if (adwParams[1] == ~0UL)
        {
            goto err;
        }

        if (adwParams[2] == ~0UL)
        {
            adwParams[2] = 1;
        }

        for (i = 0; i < adwParams[2]; i++)
        {
            if ((ret = ioctl(fd, KERRW_IOC_WRITE16, adwParams)))
            {
                printf("Error, can't Write Reg: 0x%x, ret: %d\n", adwParams[0], ret);
                return 0;
            }
            else
            {
                adwParams[0] += 4;
            }
        }
    }
    else if (!strcmp(argv[1], "r8"))
    {
        if (adwParams[0] == ~0UL)
        {
            goto err;
        }

        if (adwParams[1] == ~0UL)
        {
            adwParams[1] = 1;
        }
        adwParams[2] = adwParams[1];

        for (i = 0; i < adwParams[2]; i++)
        {
            if ((ret = ioctl(fd, KERRW_IOC_READ8, adwParams)))
            {
                printf("Error, can't Read Reg: 0x%x, ret: %d\n", adwParams[0], ret);
                return 0;
            }
            else
            {
                printf("Reg: 0x%08x, Value: 0x%08x\n", adwParams[0], adwParams[1]);
                adwParams[0] += 4;
            }
        }
    }
    else if (!strcmp(argv[1], "w8"))
    {
        if (adwParams[0] == ~0UL)
        {
            goto err;
        }

        if (adwParams[1] == ~0UL)
        {
            goto err;
        }

        if (adwParams[2] == ~0UL)
        {
            adwParams[2] = 1;
        }
        
        for (i = 0; i < adwParams[2]; i++)
        {
            if ((ret = ioctl(fd, KERRW_IOC_WRITE8, adwParams)))
            {
                printf("Error, can't Write Reg: 0x%x, ret: %d\n", adwParams[0], ret);
                return 0;
            }
            else
            {
                adwParams[0] += 4;
            }
        }
    }
    else if (!strcmp(argv[1], "dr8"))
    {
        if (adwParams[0] == ~0UL)
        {
            goto err;
        }

        if (adwParams[1] == ~0UL)
        {
            adwParams[1] = 1;
        }
        adwParams[2] = adwParams[1];

        for (i = 0; i < adwParams[2]; i++)
        {
            if ((ret = ioctl(fd, KERRW_IOC_DIRECTREAD8, adwParams)))
            {
                printf("Error, can't Read Reg: 0x%x, ret: %d\n", adwParams[0], ret);
                return 0;
            }
            else
            {
                printf("Reg: 0x%08x, Value: 0x%08x\n", adwParams[0], adwParams[1]);
                adwParams[0] += 4;
            }
        }
    }
    else if (!strcmp(argv[1], "dw8"))
    {
        if (adwParams[0] == ~0UL)
        {
            goto err;
        }

        if (adwParams[1] == ~0UL)
        {
            goto err;
        }

        if (adwParams[2] == ~0UL)
        {
            adwParams[2] = 1;
        }

        for (i = 0; i < adwParams[2]; i++)
        {
            if ((ret = ioctl(fd, KERRW_IOC_DIRECTWRITE8, adwParams)))
            {
                printf("Error, can't Write Reg: 0x%x, ret: %d\n", adwParams[0], ret);
                return 0;
            }
            else
            {
                adwParams[0] += 4;
            }
        }
    }
    else
    {
        PrintUsage(argv[0]);
    }

    return 0;

err:    
    PrintUsage(argv[0]);
    return 0;
    
}
