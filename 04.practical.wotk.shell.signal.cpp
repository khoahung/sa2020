#include <stdio.h>
#include <string.h>
#include <iostream>
#include <csignal>
#include <unistd.h>
using namespace std;
void signalHandler( int signum ) {
   cout << "Interrupt signal (" << signum << ") received.\n";

   // cleanup and close up stuff here
   // terminate program

   exit(signum);
}


int main() {
    int pid = fork();
    if (pid == 0) {
        printf("enter the command:");
        char cmd[1000];
            fgets(cmd,sizeof cmd,stdin);

        char *args[]= {"/bin/bash",cmd,NULL};
    
            if(strcmp(cmd,"quit")){
                    return 0;
        }
        signal(SIGINT, signalHandler);
        raise( SIGINT);
        execvp("/bin/bash", args);
    }else{
        printf("I am parent after fork(), child is %d\n", pid);
    }
    return 0;
}
