#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct{
    int txd[2];
    int rxd[2];
} dpipe_t;


int init_pipes(dpipe_t* dpipe){
    if ((pipe(dpipe->txd) < 0) | (pipe(dpipe->rxd) < 0))
 	{
 		printf("Pipe init error");
 		return 0;
 	}
    return 1;
}

int main(){

    dpipe_t dpipe;

    char buffer[4096];
    int size;

    if(!init_pipes(&dpipe)){
        printf("Fatal pipe errror");
    }

    const pid_t pid = fork();

 	if (pid < 0)
 	{
 		puts("Fork error");
 		return 1;
 	}

    while(1){ //Programm run loop
        if(pid){
            //Parent semantics

            // txd - parent ->-> Child
            // rxd - child ->-> Parent
            // 0 - exit 1 - entrence

            close(dpipe.txd[0]); //Closed exit of parent -> Child
            close(dpipe.rxd[1]); //Closed entrence of child -> parent

            int p_write = dpipe.txd[1];
            int p_read = dpipe.rxd[0];

            size = read(0, buffer, sizeof(buffer) - 1);
            buffer[size] = '\0';

            write(p_write,buffer,size);

            printf("Parent sent:  %s", buffer);

            if(strcmp(buffer, "exit\n") == 0){
                printf("Parent self termination\n");
                return(0);
            }

            size = read(p_read,buffer,sizeof(buffer)-1);
            buffer[size] = '\0';

             if(strcmp(buffer, "exit\n") == 0){
                printf("Parent terminated by child\n");
                return(0);
            }

            printf("Parent recieved: %s", buffer);
            
        }
        else
        {  

            // Child semantics
            
            close(dpipe.txd[1]); //Closed entrance of parent -> Child
            close(dpipe.rxd[0]); //Closed exit of child -> parent

            int c_read = dpipe.txd[0];
            int c_write = dpipe.rxd[1];

            size = read(c_read, buffer, sizeof(buffer) - 1);

            buffer[size] = '\0';


            if(strcmp(buffer, "exit\n") == 0){
                printf("Child terminated by parent \n");
                return(0);
            }

            printf("Child received:  %s", buffer);

            size = read(0,buffer, sizeof(buffer)-1);

            buffer[size] = '\0';

            write(c_write,buffer,size);

            printf("Child sent: %s", buffer);
            
             if(strcmp(buffer, "exit\n") == 0){
                printf("Child self termination\n");
                return(0);
            }
            
        }

    }

return 0;


}