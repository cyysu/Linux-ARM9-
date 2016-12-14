#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

/* ./dma_test nodma
 * ./dma_test dma
 */
#define MEM_CPY_NO_DMA  0
#define MEM_CPY_DMA     1

void print_usage(char *name)
{
	printf("Usage:\n");
	printf("%s <nodma | dma>\n", name);
}


int main(int argc, char **argv)
{
	int fd;
	
 	if (argc != 2)
	{
		print_usage(argv[0]);
		return -1;
	}

	fd = open("/dev/dma", O_RDWR);
	if (fd < 0)
	{
		printf("can't open /dev/dma\n");
		return -1;
	}

	if (strcmp(argv[1], "nodma") == 0)
	{
		while (1)
		{
			ioctl(fd, MEM_CPY_NO_DMA);
		}
	}
	else if (strcmp(argv[1], "dma") == 0)
	{
		while (1)
		{
			ioctl(fd, MEM_CPY_DMA);
		}
	}
	else
	{
		print_usage(argv[0]);
		return -1;
	}
	return 0; 	
}

