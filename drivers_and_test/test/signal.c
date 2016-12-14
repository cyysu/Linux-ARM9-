
#include <stdio.h>
#include <signal.h>

void my_signal_fun(int signum)
{
	static int cnt = 0;
	printf("signal = %d, %d times\n", signum, ++cnt);
}

int main(int argc, char **argv)
{

	signal(SIGUSR1, my_signal_fun);
	
	while (1)
	{
		sleep(1000);
	}
	return 0;
}

