#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int* global;

int main (void)
{
	pid_t f;

	global = malloc(sizeof(int));
	*global = 5;
	//printf("val = %d\n",*global);
	
	f=fork();
	if (f==-1)
	{
		printf("Error \n");
		exit(1);
	} 
	else if (f==0)
	{
		printf("   Child: global address: %d\n",global);
		printf("   Child: global before assignment = %d\n", *global);
		*global = 10;
		//printf("   Child: My process ID: %d \n", getpid());
		//sleep(1);
		printf("   Child: global = %d\n", *global);
		printf("   Child: Terminating...\n");
		//printf("   Child: My parent's process ID: %d \n", getppid());
		exit(0);
	}
	else
	{
		printf("Parent: global address: %d\n",global);
		printf("Parent: global before assignment = %d\n", *global);
		*global = 0;
		printf("Parent: global = %d\n", *global);
		//printf("Parent: My parent's process ID: %d \n", getppid());
		//printf("Parent: My process ID: %d \n", getpid());
		//printf("Parent: My child's process ID: %d \n", f);
		wait(NULL);
		printf("Parent: global after child is terminated = %d\n", *global);
		printf("Parent: Terminating...\n");
		exit(0);
	}
	return (0);
}

