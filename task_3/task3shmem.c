#include <stdio.h>
 #include <stdlib.h>
 #include <sys/ipc.h>
 #include <sys/shm.h>
 #include <sys/types.h>
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/stat.h>
 #include <time.h>
 #include <semaphore.h>
 #include <errno.h>
 #include <string.h>

const int DATA_SIZE = 1 << 25;

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

int init_shared_mem(char* shm_ptr, int data_size){
    int key_shm = rand();
    shm_ptr = shmat(shmget(key_shm, data_size + 1, IPC_CREAT | 0666), NULL, 0);
    shm_ptr[data_size] = 0;
    return 0;
}

int main() {

    

    srand(time(NULL));

    char* initial_data = malloc(DATA_SIZE);
    char* received_data = malloc(DATA_SIZE);
    // generate_data(initial_data);
   

   
   

    for(int i = 1; i < 25; i += 4){
    
        int batch_size = 1 << i;
   
        int key_shm = rand();
        
        char* shm = shmat(shmget(key_shm, batch_size + 1, IPC_CREAT | 0777), NULL, 0);
        
        shm[batch_size] = 0;
       
        int pid = fork();
        

        if(pid < 0){
            printf("Fork error\n");
            return -1;
        }

        clock_t start = clock();
   
        // Deviding logic between child and parent 
        if(!pid) {
            for(int i = 0; i < DATA_SIZE; i+= batch_size){
                
                memcpy(shm, &initial_data[i], batch_size);
                shm[batch_size] = 1;
                
                while (shm[batch_size]);        
            }
            return 1;
        }
        else
        {
            for(int i = 0; i < DATA_SIZE; i += batch_size) {
                while(!shm[batch_size]);
                memcpy(&received_data[i], shm, batch_size);
                shm[batch_size] = 0;
            }
        }

        clock_t end = clock();
        check_data_integrity(received_data,initial_data, DATA_SIZE);
        printf("%d %f\n", batch_size, (double)(end - start)/CLOCKS_PER_SEC);
    }
free(initial_data);
free(received_data);
return 0;



}