#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int ntimes=0;

main()
{
    pid_t pid, ppid;
    void p_action(int), c_action(int);
    static struct sigaction pact, cact;
    
    /* set SIGUSR1 action for parent */;
    pact.sa_handler = p_action;
    sigaction(SIGUSR1, &pact, NULL);
    
    switch (pid = fork())
    {
    case -1: // Shouldn't get here
        perror("synchro");
        exit(1);
    case 0:  // child
        cact.sa_handler = c_action;
        sigaction(SIGUSR1, &cact, NULL);
        
        // get parent process-id
        ppid = getppid();
        for (;;)
        {
            sleep(1);
            // wake up parent
            kill (ppid, SIGUSR1);
            pause(); // wait for parent signal
        }
     default: // parent
        for (;;)
        {
            pause(); // wait for child signal
            sleep(1);
            // wake up child
            kill (pid, SIGUSR1);
        }
        
    }
} // end of main

void p_action(int sig)
{
    printf("Parent caught signal %d\n", ++ntimes);
}
void c_action(int sig)
{
    printf("Child caught signal %d\n", ++ntimes);
}
