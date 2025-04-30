#include <stdio.h>

int main()
{
	int x = 5;
	int *ptr = &x;

	x = add_number(ptr);
	printf("%d", x);
}

int add_number(int number)
{
	number = number * 2;
}