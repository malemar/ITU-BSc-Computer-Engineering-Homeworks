#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define NR_set_hidden 351

int main(int argc, char **argv)
{
	long y = syscall(NR_set_hidden, atoi(argv[1]), atoi(argv[2]), 1);
	printf("\nReturn value of system call: %ld\n", y);
	if (y < 0)
	{
		perror("Error");
		switch (errno)
		{
		case EPERM:
			printf("Root privileges should be taken. Run with sudo\n");
			break;
		case EINVAL:
			printf("Flag can be 0(for OFF) or 1(for ON).\n");
			break;
		case ESRCH:
			printf("Check the PID given as parameter.\n");
			break;
		}
		printf("\n");
		return EXIT_FAILURE;
	}
	
	printf("Successful!\n\n");
	return EXIT_SUCCESS;
}