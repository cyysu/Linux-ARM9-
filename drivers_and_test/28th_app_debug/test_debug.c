
#include <stdio.h>

void C(int *p)
{
	*p = 0x12;
}


void B(int *p)
{
	C(p);
}

void A(int *p)
{
	B(p);
}

void A2(int *p)
{
	C(p);
}


int main(int argc, char **argv)
{
	int a;
	int *p = NULL;

	A2(&a);  // A2 > C
	printf("a = 0x%x\n", a);

	A(p);    // A > B > C

	return 0;
}

