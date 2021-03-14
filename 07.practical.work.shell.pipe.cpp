#include <stdio.h>
#include <string.h>
#include <iostream>
#include <csignal>
#include <unistd.h>
#include <fstream>
#include <string>
#include <iostream>
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
void doexec(void) {
        int pipefds[2];
        pipe (pipefds);
	switch (fork ()) {
		case -1: perror ("fork"); exit (1); 
		case 0:
               	 	dup2 (pipefds[1], 1);
                	close(pipefds[0]);
                	close(pipefds[1]);
		break;
	 default:
               	 	dup2(pipefds[0], 0);
                	close(pipefds[0]);
                	close(pipefds[1]);
		break;
	}
}

void saveFile(string cmd){
    std::ofstream outfile;
    outfile.open("history.txt", std::ios_base::app);
    outfile << cmd;
    outfile.close();
}
int showContentFile()
{
    std::ifstream f("history.txt");

    if (f.is_open())
        std::cout << f.rdbuf();
}
int main() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGTSTP, signalHandler);
    char cmd[1000];
    while (1) {
        cout<<"USShell> ";
        fgets(cmd,sizeof(cmd),stdin);
        if(strcmp(cmd,"/q\n")==0){
            showContentFile();
            break;
        }
        int pid = fork();
        if(pid){
            signalPid = pid;
            waitpid(pid, NULL,0);
        }else{
            doexec();
            saveFile(cmd);
            char *args[]= {"/bin/bash","-c",cmd,NULL};
            execvp("/bin/bash", args);
        }
    }
    cout<<"end";
    return 0;
}
