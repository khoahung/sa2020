#include <stdio.h>
#include <string.h>
#include <iostream>
#include <csignal>
#include <unistd.h>
using namespace std;
int signalPid;
void signalHandler( int signum ) {
   cout << "Interrupt signal (" << signum << ") received.\n";
    switch (signum) {
        case SIGTSTP:
            cout<<"Pause\n";
            kill(signalPid,SIGTSTP);
            break;
        case SIGINT:
        case SIGTERM:
            cout<<"Terminate\n";
            exit(0);
            break;
        default:
            break;
    }
}


int main() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGTSTP, signalHandler);
    char cmd[1000];
    while (1) {
        cout<<"USShell> ";
        fgets(cmd,sizeof(cmd),stdin);
        if(strcmp(cmd,"/q\n")==0) break;
        int pid = fork();
        if(pid){
            signalPid = pid;
            waitpid(pid, NULL,0);
        }else{
            char *args[]= {"/bin/bash","-c",cmd,NULL};
            execvp("/bin/bash", args);
        }
    }
    cout<<"end";
    return 0;
}
