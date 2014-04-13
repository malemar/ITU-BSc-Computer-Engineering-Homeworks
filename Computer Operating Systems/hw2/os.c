#include <stdio.h>


int main(){

	short pid;
	if((pid = fork()) == 0){
		printf("Child says hi!\n");
	} else {
		int ReturnCode;
		while(pid != wait(&ReturnCode))
			;
		printf("Parent, child is killed\n");
	}
	
	return 0;
}
