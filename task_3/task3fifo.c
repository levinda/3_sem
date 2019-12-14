#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

const int DATA_SIZE = 1 << 25;

int main(){

    srand(time(NULL));

    char* initial_data = malloc(DATA_SIZE);
    char* received_data = malloc(DATA_SIZE);

    for(int i = 0; i < DATA_SIZE; i++) {
        initial_data[i] = (char)(i%256);
        received_data[i] = 0;
    }

    int check_data_integrity(char* data1, char* data2, int size){
         for(int i = 0; i < size; i++) {
                if(data1[i] != data2[i]){
                    return 0;
                }
             }
        return 1;
    }


    for(int i = 1; i < 25; i += 4){

        int buffer_size = 1 << i;

         int fifo[2];
         pipe(fifo);

        int pid = fork();

         if(pid < 0){
            printf("Fork error\n");
            return -1;
        }

        clock_t start = clock();

        if(!pid) {
            for(int i = 0; i < DATA_SIZE; i += write(fifo[1], &initial_data[i], buffer_size));
                return 1;
            }
        else {
            for(int i = 0; i < DATA_SIZE; i += read(fifo[0], &received_data[i], buffer_size));
            }
        clock_t end = clock();


        check_data_integrity(received_data,initial_data, DATA_SIZE);
        printf("%d %f\n", buffer_size, (double)(end - start)/CLOCKS_PER_SEC);
       
    }

    free(initial_data);
    free(received_data);
    return 0;

}
