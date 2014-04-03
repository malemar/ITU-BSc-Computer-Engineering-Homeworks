#include <stdio.h>          /* printf()                 */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */


int main(){
	FILE *fp;
	int pid;
	char buf[128];
	
	char buf1[128];
	int dumm;
	char buf2[128];
	
	printf("%d\n", (int)getpid());
	printf("%d\n", (int)getppid());
	
	pid = getppid();
	
	while (pid > 1){
		sprintf(buf, "/proc/%d/stat", (int)pid);
		fp = fopen(buf, "r");
		if (fp == NULL) break;
		fscanf(fp, "%d %s %s %d", &dumm, buf1, buf2, &pid);
		fclose(fp);
		printf("%d\n", (int)pid);
	}
	
    while(1) pause() ;
    return 0;
}
