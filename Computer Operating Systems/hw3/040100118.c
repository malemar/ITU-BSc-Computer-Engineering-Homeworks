/****************************************/
/* BLG312E - Computer Operating Systems */
/*				Spring 2013				*/
/*			   Homework III				*/
/*										*/
/*			Volkan İlbeyli				*/
/*			   040100118                */
/*										*/
/*		Insturctor: Şima Etaner Uyar	*/
/****************************************/

#include <stdio.h>		/* printf(), fopen(), fclose()	*/
#include <stdlib.h>		/* exit(), malloc(), free()		*/
#include <sys/types.h> 	/* key_t, sem_t, pid_t 			*/
#include <sys/shm.h> 	/* shmat(), IPC_RMID			*/ 
#include <errno.h> 		/* errno, ECHILD 				*/
#include <semaphore.h>	/* sem_open(), sem_destroy(), sem_wait(), sem_post()	*/
#include <fcntl.h>		/* O_CREAT, O_EXEC				*/

/*
	In this homework, the <semaphore.h> library is used
	instead of <sys/sem.h> since it's easier and simpler to use.
	<semaphore.h> is the POSIX implementation of semaphores
	while <sys/sem.h> is the Unix System V implementation.
	<semaphore.h> comforms to POSIX standards. 
	a few functions from library are below:
	
		sem_init(&semaphore, pshared, init) -> 	semaphore's address
												shared variable: 1 => shared among processes
																 0 => just 1 process
												init: 			initial value of the semaphore
												
		sem_open(const char *name, int oflag,	
					 mode_t mode, unsigned int value); 
					 
					 parameters:
					   name of the semaphore (these semaphores are called names POSIX semaphores)
					   flags -> O_CREAT etc.
					   mode  -> permissions
					   value -> semaphore value. 
					 
					 sem_open is used for semaphores which processes share.
					 using sem_init() by its second variable being non zero is not sufficient
					 alone. After that, an mmap() has to be called. For more information:
					 http://man7.org/linux/man-pages/man3/sem_open.3.html
					 http://man7.org/linux/man-pages/man3/sem_init.3.html
					 
		sem_destroy(&semaphore)				-> destroys the semaphore
		sem_wait(&semaphore)				-> P operation
		sem_trywait(&semaphore)				-> conditional P operation
		sem_post(&semaphore)				-> V operation
		
	semaphore is of type sem_t. 
	All the functions return an integer error code stored in errno except sem_open().
	
	compile: gcc 040100118.c -lpthread
	run:	./a.out in.txt out.txt
*/	
/*	
	NOTE: 	Except for FIFO approach, everything works just *perfect*.
			I could not find an easy work-around for FIFO mechanism
			based on the system I designed. It required either more
			variables that will be shared, or a more sophisticated
			system design.
			
			Below is a summary of how the program works:
			- Input file is read into the boxIn[] array.
			- colorSequence array is initialized with the same size
			  as the boxIn[] array. This array will contain each
			  letter once. Therefore it's redundant. It will be
			  freed, but before free() its contents will be transferred
			  to queue[] array. (realloc() might come much more
			  handier in this situation.)
			- Each color's appearance is counted and written to
			  colorCount array. Colors have the same index as they
			  have in queue[] array.
			- A pointer *p is initialized in the shared memory with
			  the value of queue[0]. Based on this pointers value,
			  processes will wait their turn. This pointer shows
			  the current color of the painting machine.
			- A shared integer *count is initialized in the shared 
			  memory with initial value being 0. This variable is
			  used to compare the currently painted box count with
			  the total number of boxes that have the same color and
			  stored in the colorCount[] array. Therefore in every
			  color change of the machine, this counter is reset.
			- Another shared integer nColorChange is stored in shared
			  memory to keep track of the total number of color changes
			  made by the painting machine. This value is incremented
			  in every color change situation.
			- Semaphores are initialized.
			- Children are forked.
			- Parent waits until children are finished. Then it updates
			  the output file with the nColorChange value being added
			  to beginning of the file. Then it deallocates the resources
			  and exits.
			- Child processes wait their turn based on the value of p.
			  Once they get their turn, they paint the box, write to
			  the output file.

*/
/* function prototypes */
int contains(char*, unsigned int, char);		
unsigned int indice(char*, unsigned int, char);
						
int main(int argc, char** argv) {
	unsigned int n;				/* 	number of boxes			*/ 
	unsigned int nColor;		/*	# different colors		*/
	char* colorSequence;		/* 	color sequence(tmp) 	*/
	char* queue;				/* 	color queue 	 		*/
	unsigned int* colorCount;	/* 	count of each color	 	*/
	int i,j;					/* 	loop variables 			*/
    FILE* fp;   				/*  file pointer 			*/
    char* boxIn;				/* 	Box input sequence 		*/ /*shared*/
    key_t shmkey;				/* 	shared memory key 		*/
	int shmid;					/* 	shared memory id 		*/
	sem_t* sem; 				/* 	synch semaphore			*/ /*shared*/
	sem_t* mutex;				/*  mutex for *p and count	*/ /*shared*/  
	unsigned int value = 2;		/* 	semaphore value 		*/
	pid_t pid;					/*	fork pid				*/
	char* p;					/* 	color queue pointer		*/ /*shared*/
	int* count;					/*  paint operation counter	*/ /*shared*/
	unsigned int* nColorChange;	/*  #of color changes		*/ /*shared*/ 	
	char* buffer;				/* 	file update buffer		*/
	long fsize;					/* 	file size 				*/
	
	/*****************************************************/
	/***************** ERROR CHECK ***********************/
	/*****************************************************/
    /* main parameter check */
    if(argc != 3){  
        printf("Invalid argument count.\n");
        printf("Example: ./a.out in.txt out.txt\n");
        printf("Returning from main()...\n");
        return -1;
    }
    
    /* open file */
    fp = fopen(argv[1],"r");
    if(fp == NULL){ /* check for errors */
        printf("Error opening file: %s\n", argv[1]);
        printf("Returning from main()...\n");
        return -1;
    }
    
    /******************************************************/
	/************	PREPARE RESOURCES	*******************/
	/******************************************************/
    /* get n */
    fscanf(fp,"%d\n",&n);
    

    /*  It is not necessary to implement this array on
     *  shared memory since the processes will only read
     *  this array and learn their color based on the index
     *  in which they are forked. However, this array is 
     *  implemented on shared memory since we are asked to do so. 
     */
     
    /* initialize boxIn in shared memory */     
    shmkey = ftok("/dev/null",3);   /* executable name and a random number */
    /*printf("shmkey for boxIn = %d\n",shmkey);*/
    shmid = shmget(shmkey, sizeof(char), 0700 | IPC_CREAT);
    if(shmid < 0){      /* shared memory error check */
        perror("shmget\n"); 
        exit(1);
    }
    boxIn = (char*) shmat(shmid, NULL, 0);
    printf("\nboxIn allocated in shared memory.\n");
    
 	/* read the input file */   
    for(i=0; i<n ; i++)
    	fscanf(fp,"%c\n", &boxIn[i]);
    fclose(fp);
    printf("input read.\n");
    
    /* create output file */
    fp = fopen(argv[2],"w");
    fclose(fp);
    printf("output file created.\n");
    
    /*******************************************************/
    
    /* create color sequence array(queue) */
    nColor = 0;
    colorSequence = (char*) malloc(n+1);	/* too much space allocated */
    p = &colorSequence[0];				/* it will be freed asap	*/
    for(i=0; i<n ; i++){
    	if(contains(colorSequence, n, boxIn[i]) == 0){
    		nColor++;
    		*p = boxIn[i];
    		p++;
    	}
    }
    p = NULL;
    queue = (char*) malloc(nColor);
    for(i=0; i<nColor ; i++)	/* copy color sequence into queue */
    	queue[i] = colorSequence[i]; 
    free(colorSequence);	/* return color sequence back to heap */
    printf("color queue created.\n");
    
    /* print all boxes and color sequence */
    printf("\nBoxes:\n");
    for(i=0; i<n ; i++)
    	printf("%c ",boxIn[i]);
    printf("\nColor type count: %d\n",nColor);
    
    printf("\nColor queue:\n");
    for(i=0; i<nColor ; i++)
    	printf("%c ",queue[i]);
    printf("\n\n");
    
    /* count every color's appearance */
    colorCount = (int*) malloc(nColor*sizeof(int));	/* allocate space */
    
    for(i=0; i<nColor ; i++){	/* for each color */
    	for(j=0; j<n ; j++){	/* search boxIn for appearances */
    		if(boxIn[j] == queue[i])
    			colorCount[i] += 1;
    	}
    }
    
    /* print color appearances(counts) */
    printf("Color appearance counts:\n");
    for(i=0; i<nColor ; i++)
    	printf("%c: %d\n",queue[i],colorCount[i]);
	printf("\n");
    
    /*******************************************************/
    
    /* initialize color sequence in shared memory */
    shmkey = ftok("/dev/null",4);	/* executable name and a random number */
    /*printf("shmkey for count = %d\n",shmkey);*/
	shmid = shmget(shmkey, sizeof(int), 0700 | IPC_CREAT);
	if(shmid < 0){		/* shared memory error check */
		perror("shmget error\n"); 
		exit(1);
	}
    count = (int*) shmat(shmid, NULL, 0);	/* shared memory part of colorSequence */
    *count = 0;
    printf("count pointer allocated in shared memory.\n");
    
    /* initialize color sequence pointer in shared memory */
    shmkey = ftok("/dev/null",5);	/* executable name and a random number */
    /*printf("shmkey for p = %d\n",shmkey);*/
	shmid = shmget(shmkey, sizeof(char), 0700 | IPC_CREAT);
	if(shmid < 0){		/* shared memory error check */
		perror("shmget\n"); 
		exit(1);
	}
    p = (char*) shmat(shmid, NULL, 0);	/* pointer to queue in shared memory */
    printf("queue pointer allocated in shared memory.\n");
    *p = queue[0];
    
    /* initialize # color changes in shared memory */
    shmkey = ftok("/dev/null",'V');	/* executable name and a random number */
    /*printf("shmkey for ncolor = %d\n",shmkey);*/
	shmid = shmget(shmkey, sizeof(char), 0700 | IPC_CREAT);
	if(shmid < 0){		/* shared memory error check */
		perror("shmget\n"); 
		exit(1);
	}
    nColorChange = (int*) shmat(shmid, NULL, 0);	/* # color changes in shared memory */
    *nColorChange=0;
    printf("#colorChanges=%d allocated in shared memory.\n",*nColorChange);
    
    /********************************************************/
    
    /* initialize semaphores for shared processes */
	sem = sem_open("painting", O_CREAT|O_EXCL, 0, value);	/* value=2 */
	mutex = sem_open("sharedvar", O_CREAT|O_EXCL, 0, 1);
	sem_unlink("painting");	 /* unlink prevents the semaphore existing forever 	*/
	sem_unlink("sharedvar"); /* if a crash occurs during the execution 			*/
	/* http://stackoverflow.com/questions/6659780/semaphore-equivalent-for-processes/6659866#6659866  */
	/* read comment section for sem_unlink() */
	printf("semaphores initialized.\n\n");


    /* fork child processes */
    for(i=0; i<n ; i++){
    	pid = fork();
    	if(pid < 0)	/* check for error	*/
    		printf("Fork error.\n");
    	else if(pid == 0) break;	/* child processes */
    }
    
    
    /******************************************************/
    /******************   PARENT PROCESS   ****************/
    /******************************************************/
	if(pid != 0){
		/* wait for all childs to exit */
		while (pid = waitpid(-1, NULL, 0)) {
			if (errno == ECHILD)	
				break;
		}
		
    	printf("\nAll paints are done.\n");
		printf("#ColorChange=%d\n",*nColorChange);
		
		/* update the output file */
		/* read file into buffer */
		fp = fopen(argv[2],"r");
		fseek(fp, 0, SEEK_END);
		fsize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		buffer = malloc(fsize + 1);
		fread(buffer, fsize, 1, fp);
		fclose(fp);
		/* add the colorChange # */
		fp=fopen(argv[2],"w");
		fprintf(fp, "%d\n", *nColorChange);
		fprintf(fp, "%s", buffer);
		fclose(fp);
		
		/* shared memory detach */
		shmdt(p);
		shmdt(count);
		shmctl(shmid, IPC_RMID, 0);
	
		/* dynamic memory returned back to heap */
		free(colorCount);
		free(queue);
	
		/* cleanup semaphores */
		sem_destroy(sem);    	
		sem_destroy(mutex);
		exit(0);
    }
    
    /******************************************************/
    /******************   CHILD PROCESS   *****************/
    /******************************************************/
    else{
    	/*printf("%c %d forked. *p=%c, *count=%d\n",boxIn[i],i,*p,*count);*/
    	while(*p != boxIn[i]); 	/* wait until it's my turn */
    	
    	sem_wait(sem);			/* P operation */
    	printf("%c box(%d) is being painted.\n",boxIn[i],i);
    	if(boxIn[i] == 'R' || boxIn[i] == 'Y' || boxIn[i] == 'P')
    		sleep(1);
    	if(boxIn[i] == 'G' || boxIn[i] == 'B' || boxIn[i] == 'O')
    		sleep (2);
    	printf("Done painting(%d).\n",i);
    	
    	sem_wait(mutex);		/* P operation */
    	(*count)++;
    	fp = fopen(argv[2],"a");	/* write to file*/
    	fprintf(fp,"%d %c\n",i,boxIn[i]);
    	fclose(fp);			/* close file */
    	
    	/* arrange queue */
		if(*p==boxIn[i]){	/* if its my turn, and */
			/* if all the boxes same as my color are painted */
			if(*count==colorCount[indice(queue,nColor,boxIn[i])]){
    			/* move pointer p to next color */
    			j = indice(queue,nColor,boxIn[i]); /* current colors indice in queue[] */
    			j++;	/* move to enxt indice */
    			if(j != nColor)	/* if its in the bounds of queue[] array */
    				*p = queue[j];	/* assign p the next color */
    			
    			/* reset the colored box count */
    			*count = 0;
    			
    			/* increment # color changes */
    			(*nColorChange)++;
    		}
    	}
    	sem_post(mutex);	/* V operation */
    	sem_post(sem);		/* V operation */
    	exit(0);
    }
    
}

int contains(char* array, unsigned int size, char ch){
	int i;		/* loop variable */
	for(i=0 ; i<size ; i++){
		if(array[i] == ch)	return 1;
	}
	return 0;
}

unsigned int indice(char* array, unsigned int size, char ch){
	unsigned int i;
	for(i=0; i<size ; i++){
		if(ch==array[i])
			return i;
	}
}
