
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

//Number of threads

const int nthreads = 1;
const int dots = 1 << 26;

const double left = 0;
const double right = 1;

const long big = 1000000;



typedef struct{
    int dots;
    double x_min;
    double x_max;
    double result;
} thread_task;


// Function to integrate, do not input 0 or area edges with different signs.
double f(double x){
    return cos(x);
}

double random_number_from_0_to_1(){
	
	return drand48();
}


void* calculate_task(void* args){

    thread_task * task = (thread_task*) args;

    double temp_result = 0;



    double inter_len = task->x_max - task->x_min;
    double x, y;

    for(int i = 0; i < task->dots; i++){
        x = task->x_min + random_number_from_0_to_1() * inter_len;
        y = f(x);
        temp_result += y;
    }

    task->result = (temp_result * inter_len)/ task->dots;

    pthread_exit(NULL);
}



int main(){


    for (int n=1; n<50; n++){

    pthread_t thread[n];
    int dots_to_each_thread = dots / n;
    double interval = (right - left) / (double)n;

	
	struct timespec start,stop;
		
    thread_task tasks[n];

    for(int i = 0; i < n; i++){
        tasks[i].x_min = left + interval * i;
        tasks[i].x_max = left + interval * (i+1);
        tasks[i].dots = dots_to_each_thread;
    }

    //Creating threads

    if(clock_gettime(CLOCK_MONOTONIC, &start) == -1)
	{
		perror("clock_gettime_error");
		return -1;
	}

    for (int i = 0; i < n; i++)
        pthread_create(&thread[i], NULL, calculate_task, &tasks[i]);
    for (int i = 0; i < n; i++)
        pthread_join(thread[i], NULL);

    if(clock_gettime(CLOCK_MONOTONIC, &stop) == -1)
	{
			perror("clock_gettime_error");
			return -1;
	}
	
	double time_sec = (stop.tv_sec - start.tv_sec) +(double)(stop.tv_nsec-start.tv_nsec)/1000000000;
    double result = 0;
    for (int i = 0; i < n; i++)
    {
        result += tasks[i].result;
    }

    printf("%d %f,%f\n", n, result, time_sec);
    }

    return(0);
}


