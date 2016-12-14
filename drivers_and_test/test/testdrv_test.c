#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <unistd.h>
#include <fcntl.h>

int fd;

void my_sig_fun(int signo)
{
    //static int cnt = 0;
    //printf("Get signal: %d, %d times\n", signo, ++cnt);

	unsigned char key_val;
	
	read(fd, &key_val, 1);
	printf("key_val = 0x%x\n", key_val);
}

int main(int argc, char **argv)
{
	int flags;
	fd = open("/dev/buttons", O_RDWR);
	if (fd < 0)
	{
		printf("can't open!\n");
		return -1;
	}

	fcntl(fd, F_SETOWN, getpid());

	flags = fcntl(fd, F_GETFL); 
	fcntl(fd, F_SETFL, flags | FASYNC);
	
	
    signal(SIGIO, my_sig_fun);
    while (1)
    {
        sleep(1000);
    }
    return 0;
}

