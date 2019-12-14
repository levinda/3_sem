
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

//Number of threads

const int nthreads = 1;
const int dots = 1 << 27;

const double left = 1.0;
const double right = 5;

const long big = 10000000000;



typedef struct{
    int dots;
    double x_min;
    double x_max;
    double result;
} thread_task;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


// Function to integrate, do not input 0 or area edges with different signs.
double f(double x){
    return exp(-x) * x;
}

double random_number_from_0_to_1(struct drand48_data* buff){
    long int randomValue;
 	lrand48_r(buff,&randomValue);
 	return (double)(randomValue%(big))/(double)big;
}


void* calculate_task(void* args){

    thread_task * task = (thread_task*) args;

    double temp_result = 0;

    struct drand48_data buff;
 	srand48_r(rand(), &buff);


    double inter_len = task->x_max - task->x_min;
    double x, y;

    for(int i = 0; i < task->dots; i++){
        x = task->x_min + random_number_from_0_to_1(&buff) * inter_len;
        y = f(x);
        temp_result += y;
    }

    task->result = (temp_result * inter_len)/ task->dots;

    pthread_exit(NULL);
}



int main(){


    srand(time(NULL));

    for (int n=1; n<50; n++){

    pthread_t thread[n];
    int dots_to_each_thread = dots / n;
    double interval = (right - left) / (double)n;

    thread_task tasks[n];

    for(int i = 0; i < n; i++){
        tasks[i].x_min = left + interval * i;
        tasks[i].x_max = left + interval * (i+1);
        tasks[i].dots = dots_to_each_thread;
    }

    //Creating threads

    clock_t start = clock();

    for (int i = 0; i < n; i++)
        pthread_create(&thread[i], NULL, calculate_task, &tasks[i]);
    for (int i = 0; i < n; i++)
        pthread_join(thread[i], NULL);

    clock_t end = clock();

    double result = 0;
    for (int i = 0; i < n; i++)
    {
        result += tasks[i].result;
    }

    printf("%d,%f,%f\n",n,result, (double)(end - start)/CLOCKS_PER_SEC);
    }

    return(0);
}









