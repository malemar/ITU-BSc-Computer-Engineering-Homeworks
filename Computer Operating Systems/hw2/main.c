/****************************************/
/* BLG312E - Computer Operating Systems	*/
/*				Spring 2013				*/
/*										*/
/*			Volkan İlbeyli				*/
/*			   040100118                */
/*										*/
/*		Insturctor: Şima Etaner Uyar	*/
/****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include <sys/types.h> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>

/*
	In this homework, the <semaphore.h> library is used
	instead of <sys/sem.h> since it's easier to use.
	<semaphore.h> is the POSIX implementation of semaphores
	while <sys/sem.h> is the Unix System V implementation.
	<semaphore.h> a few functions:
	
		sem_init(&semaphore, pshared, init) -> 	semaphore's address
												shared variable: 1 => shared among processes
																 0 => just 1 process
												init: initial value of the semaphore
		sem_destroy(&semaphore)				-> destroys the semaphore
		sem_wait(&semaphore)				-> P operation
		sem_trywait(&semaphore)				-> conditional P operation
		sem_post(&semaphore)				-> V operation
		
	semaphore is of type sem_t. All the functions return an integer error code.
	compile: gcc main.c -lpthread -lm
	run:	./a.out
*/										
void c_action(int sig);					
static volatile sig_atomic_t sigusr1 = 0;
int main(int argc, char *argv[]){
	/* please read final comments at the end of the file before proceeding */
	int i,j;				/* loop variable */
	int m, n;				/* program parameters */
	int* array; 			/* input array */
	float *result;			/* output arrays */
	FILE* fp;				/* file pointer */
	key_t shmkey;			/* shared memory key */
	int shmid;				/* shared memory id */
	sem_t sem; 				/* synch semaphore */
	int pshared = 1;		/* shared attribute 1 -> multiple processes */
	unsigned int value = 1;	/* semaphore value */
	pid_t pid;				/* child pid */
	int element_count, sum;
	float mean, stddev;		/* mean and stddev 	*/
							/* standard deviation is population std dev */
	int* childs;			/* child process pid array */
	static struct sigaction cact;	/* child sigaction */
	/******************************************************/
	/****************	ERROR CHECKING	*******************/
	/******************************************************/
	if(argc < 5){	/* parameter count check */
		printf("Error! Invalid parameter count\n");
		printf("Parameters should be as follows: m, n, inputFileName, outputFileName\n\n");
		printf("Returning from main() . . . \n");
		return -1;
	}
	
	/* converting first 2 arguments into integers */
	m = atoi(argv[1]);
	n = atoi(argv[2]);
	
	if(m >= n){	/* checking condition m < n */
		printf("Error! Invalid parameter value\n");
		printf("m and n should satisfy the condition \" m < n \"");
		printf("Returning from main() . . . \n");
		return -1;
	}
	
	/******************************************************/
	/**************	PREPARING RESOURCES	*******************/
	/******************************************************/
	/* shared memory allocation */
	shmkey = ftok("/dev/null",1);	/* executable name and a random number */
	shmid = shmget(shmkey, 2*m*sizeof(float), 0666 | IPC_CREAT);
	if(shmid < 0){
		perror("shmget\n"); 
		exit(1);
	}
	result = (float*) shmat(shmid, NULL, 0); 
	/* dynamic allocation for array[n] and result[m] */
	array = (int *) malloc(n * sizeof(int));		
	//result = (float *) malloc(2 * m * sizeof(float));	/* since its not possible to attach 2D arrays 	*/
														/* into a shared memory segment, result array 	*/
														/* will be 1D array, [0-(m-1)] containing mean, */
														/* [m-(2m-1)] containing standard deviation 	*/
	/* read from file, fill up the array */
	fp = fopen(argv[3],"r");
	if(fp == NULL){	/* error check */
		printf("Error opening file: %s\nReturning from main(). . .\n", argv[3]);
		return -1;
	}
	for(i=0 ; i<n ; i++){ /* reading into array */
		fscanf(fp, "%d\n", &array[i]);
	}
	fclose(fp);

	/* semaphore initialization */
	if((sem_init(&sem, pshared, value)) == 1){
		perror("Error initializing synch semaphore\n");
		exit(1);
	}
	printf("Parent process prepared resources. . .\n");
	sleep(1);
	
	/******************************************************/
	/***  CREATE CHILD PROCESSES AND PROCESS THE INPUT	***/
	/******************************************************/
	childs = (int*) malloc(m*sizeof(int));
	for(i=0 ; i<m ; i++){	/* fork child processes */
		pid = fork();
		if(pid < 0){	/* error check */
			printf("fork() error! iteration: %d\n", i);
			exit(1);
		}
		else if(pid == 0)
			break;	
		else{
			sleep(1);
			//printf("i = %d\n",i);
			childs[i] = pid;
		}
	}
	
	/******************************************************/
	/*****************  PARENT PROCESS	*******************/
	/******************************************************/
	if(pid != 0){
		/* wake the children */
		for(i=0 ; i<m ; i++){
			kill(childs[i],SIGUSR1);
		}
	
		printf("Signals are sent, parent is waiting for childs to exit()\n");
		wait(NULL);
		//while(wait(NULL) > 0){;}	/* wait for child processes to terminate */
		result = (float*) shmat(shmid, NULL, 0);
		printf("PARENT PRINTING RESULTS\n");
		for(i=0 ; i<2*m ; i++){
			if(i<m)
				printf("%f\t",result[i]);
			if(i == m-1)
				printf("\n");
			if(i>=m)
				printf("%f\t",result[i]);
			printf("\n");
		}
		fp = fopen(argv[4],"w");
		if(fp == NULL){	/* error check */
			printf("Error opening file: %s\nReturning from main(). . .\n", argv[4]);
			return -1;
		}
		for(i=0 ; i<2*m ; i++){
			if(i<m)
				fprintf(fp,"%f\t",result[i]);
			if(i == m-1)
				fprintf(fp,"\n");
			if(i>=m)
				fprintf(fp,"%f\t",result[i]);
		}
	
		/******************************************************/
		/*********************	CLEANUP	 **********************/
		/******************************************************/
		/* shared memory detach */
		shmdt(result);
		shmctl(shmid, IPC_RMID, 0);
	
		/* dynamic memory returned back to heap */
		free(array);
		free(childs);
	
		/* cleanup semaphores */
		sem_destroy(&sem);
	
	
		return 0;
	}
	/******************************************************/
	/******************  CHILD PROCESS	*******************/
	/******************************************************/
	if(pid == 0){	/* child process does its thing */
		printf("Child #%d with pid:%d created and going to sleep...\n",i+1,getpid());
		cact.sa_handler = c_action;
		sigaction(SIGUSR1, &cact, NULL);
		pause();
		//printf("Child #%d with pid:%d is awaken\n",i+1,getpid());
		shmid = shmget(shmkey, 2*m*sizeof(float), 0);
		if(shmid < 0){
			perror("shmget\n"); 
			exit(1);
		}
		result = (float*) shmat(shmid, NULL, 0);
		sem_wait(&sem);	/* P */
		sum=0; stddev=0;
		if(i!=0){	/* get element count for each process */
			element_count = ((int) n/(i+1)) + 1;	/*	3rd process, index 2 therefore index+1 = order		 */
													/* +1 after division -> example: n=11, i=3, 11/3 = 3	 */
													/*	element_count should be 4 since array[0],[3],[6],[9] */	
													/*	therefore +1	*/
		}
		else
			element_count = n;
			
		printf("Child #%d: element count=%d ",i+1,element_count);
		for(j=0 ; j<element_count ; j++)
			printf("%d ",j*(i+1));
		printf("\n");
		
		/* find mean value */
		for(j=0 ; j<element_count ; j++)
			sum += array[j*(i+1)];
		mean = (float) sum/element_count;
		
		/* find std dev, first find the summation, then take sqrt */
		for(j=0 ; j<element_count ; j++)
			stddev += (mean-array[j*(i+1)])*(mean-array[j*(i+1)]);	/* square of difference */
		stddev /= element_count;	/* divided by element count */
		stddev = sqrt(stddev);		/* square root taken */
		
		printf("Child #%d: sum=%d , mean=%f, stddev:%f\n",i+1,sum,mean,stddev);
		
		/* write to array */
		result[i] = mean;
		result[i+m] = stddev;
		printf("Child #%d has written to array: result[%d] = %f , result[%d] = %f\n\n",i+1 ,i, result[i], i+m, result[i+m]);
		sleep(1);
		
		sem_post(&sem);	/* V */
		shmdt(result);
		exit(0);
	}
}

void c_action(int sig)	//child action when the signal is caught
{
    //printf("Child caught signal\n");
}

/*	
	FINAL NOTES
 	The killing of child processes were not
 	necessary since the child processes simply exit
 	when their task is complete. Therefore a killing
 	mechanism is not implemented.
 	
 	The outputfile contains 2 rows:
 	1) mean values 
 	2) standard deviations
 	
 	sample input file:
	2
	4
	4
	4
	5
	5
	7
	9
 	
 	n=8, m=5
 	
 	output file:
	5.000000	3.600000	4.333333	2.333333	3.500000	
	2.000000	2.416609	2.054805	2.054805	1.500000		

*/
