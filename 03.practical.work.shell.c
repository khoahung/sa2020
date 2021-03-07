#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main() {
	int pid = fork();
	if (pid == 0) {
		printf("enter the command:");
		char cmd[1000];
        	scanf("%[^\n]s", cmd);

		char *args[]= {"/bin/bash",cmd,NULL};
	
        	if(strcmp(cmd,"quit")){
                	return 0;
		}
		execvp("/bin/bash", args);		
	}else{
		printf("I am parent after fork(), child is %d\n", pid);
	}
	return 0;
}
