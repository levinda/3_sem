
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

char ** parse_command(char * cmd) {
   char *p;
   int i = 0;
   char delim[] = " \n";
   char ** parsed = (char **)malloc(100 * sysconf(_SC_ARG_MAX) / 2 * sizeof(char));
   for (p = strtok(cmd, delim); p != NULL; p = strtok(NULL, delim)) {
	   parsed[i++] = p;
   }
   parsed = realloc(parsed, i);
   return parsed;
}

static void run_cmd(char *cmd) {
   const pid_t pid = fork();
   int status;
   if (pid < 0) {
	   printf("fork() error\n");
   }
   if (pid) {
	   waitpid(pid, &status, 0);
	   return;
   }
   char ** args = parse_command(cmd);
   int err = execvp(args[0], args);
   free(args);
   if (err == -1) {
	   printf("exec() error\n");
	   exit(1);
   }
   exit(0);
}

int read_cmd(char *cmd){



    if (fgets(cmd, 256, stdin) == NULL)
    {
      printf("Invalid string");
      return(0);
    }else{
       return(1);
    }

}



int main() {
   const long size_max = sysconf(_SC_ARG_MAX);
   while(1) {
	   char *cmd;
	   cmd = (char *)malloc(size_max * sizeof(char));
      if(!read_cmd(cmd)){
         while(!read_cmd(cmd)){
            
         }
      } 
	   if (!strcmp(cmd, "exit\n") || !strcmp(cmd, "exit")) {
		   free(cmd);
		   return(0);
	   }
	   cmd = realloc(cmd, strlen(cmd) + 1);
	   run_cmd(cmd);
	   free(cmd);
   }
   return 0;
}
