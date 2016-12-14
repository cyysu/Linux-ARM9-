
#include <stdio.h>
#include <unistd.h>

int cnt = 0;

void C(void)
{
	int i = 0;

	while (1)
	{
		printf("Hello, cnt = %d, i = %d\n", cnt, i);
		cnt++;
		i = i + 2;
		sleep(5);
	}
}

void B(void)
{
	C();
}


void A(void)
{
	B();
}

int main(int argc, char **argv)
{
	A();
	return 0;
}

