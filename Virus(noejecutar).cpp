#include "unistd.h"
#include "iostream"
#include "stdlib.h"
#include "stdio.h"
#include "sys/types.h"
#include "wait.h"

using namespace std;

int main()
{
	pid_t dir;
	while(1)
	{
		dir = fork();
		cout <<"hola" <<endl;
	}
	return 0;
}
