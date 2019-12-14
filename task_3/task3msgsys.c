#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>


const int DATA_SIZE = 1 << 25;

typedef struct {
    long mtype;
    char mtext[1000000];
} msg_buff;


void generate_data(char* data){
    for(int i = 0; i < DATA_SIZE; i++){
        data[i] = (char)(i%256);
    }
}

int check_data_integrity(char* data1, char* data2, int size){
     for(int i = 0; i < size; i++) {
        if(data1[i] != data2[i]){
            return 0;
        }
    }
    return 1;
}

int init_msg( int* msg){
    int key_msg = rand();
    *msg = msgget(key_msg, IPC_CREAT | 0666);
    if(msg < 0){
        puts("Init msg error\n");
        return 0;
    }
    return 1;
}


int main() {
     srand(time(NULL));

    char* initial_data = malloc(DATA_SIZE);
    char* received_data = malloc(DATA_SIZE);

    for(int i = 0; i < DATA_SIZE; i++) {
        initial_data[i] = (char)(i%256);
        received_data[i] = 0;
    }

    for(int i = 1; i <25; i += 4){
        
        int msg;

        if(!init_msg(&msg)){
            return(-1);
        }

        msg_buff m_buffer;
        m_buffer.mtype = 1;

        int buffer_size = 1 << i;
        
        clock_t start = clock();

        int pid = fork();
        

        if(pid < 0){
            printf("Fork error\n");
            return -1;
        }
 

        if(!pid) {
            for(int i = 0; i < DATA_SIZE; i += buffer_size) {
                memcpy(m_buffer.mtext, &initial_data[i], buffer_size);
                if(msgsnd(msg, &m_buffer, buffer_size, 0) < 0) {
                    puts("Send error\n");
                    return -1;
                }
            }  
            return 1;
        }
        else
        {
            for(int i = 0; i < DATA_SIZE; i += buffer_size) {
                if(msgrcv(msg, &m_buffer, buffer_size, 1, 0) < 0) {
                    printf("msgrcv error\n");
                    return -1;
                }
                memcpy(&received_data[i], m_buffer.mtext, buffer_size);
            }
        }
        clock_t end = clock();

        check_data_integrity(initial_data,received_data, DATA_SIZE);
        printf("%d %f\n", buffer_size, (double)(end - start)/CLOCKS_PER_SEC);
    
    }

free(initial_data);
free(received_data);
return 0;


}