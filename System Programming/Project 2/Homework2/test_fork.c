#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define NR_set_hidden 351

int main(int argc, char **argv)
{
	long y;
	printf("PID of current process: %d\n\n", getpid());
	
	printf("Call set_hidden system call to set flag 1\n");
	y = syscall(NR_set_hidden, getpid(), 1);
	printf("Return value of set_hidden system call: %ld\n", y);
	if (y < 0)
	{
		printf("set_hidden system call failed. Run with sudo\n");
		return EXIT_FAILURE;
	}
	printf("Call fork system call\n");
	y = fork();
	printf("Return value of fork system call: %ld\n", y);
	
	printf("\n");
	
	printf("Call set_hidden system call to set flag 0\n");
	y = syscall(NR_set_hidden, getpid(), 0);
	printf("Return value of set_hidden system call: %ld\n", y);
	printf("Call fork system call\n");
	y = fork();
	
	if (y > 0)
	{
		printf("Parent: Return value of fork system call: %ld\n", y);
		wait(0);
	}
	else
	{
		printf("Child: Return value of fork system call: %ld\n", y);
		exit(0);
	}
	
	return EXIT_SUCCESS;
}