#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <unistd.h>
#include <algorithm>
#include <sched.h>
#include <signal.h>

using namespace std::chrono;
using std::sort;

// declare area
// void *do_one_thing(void *);
// void *do_another_thing(void *);
// void do_wrap_up(int, int);
void pinCPU (int cpu_number);
void setSchedulingPolicy (int newPolicy, int priority);
static void workload_1ms (void);


// define area

#define CPU_CORE 0
#define DEFAULT_PRIORITY 0
#define TASK_NUMBER 3

#define WORKLOADTIME_TAU1 1
#define PERIODTIME_TAU1 50000

#define WORKLOADTIME_TAU2 50
#define PERIODTIME_TAU2 10000

#define WORKLOADTIME_TAU3 10
#define PERIODTIME_TAU3 5000

int r1 = 0, r2 = 0;
int task_number = TASK_NUMBER;

struct tau_parameter
{
    int exectime, period, priority;
    bool operator < (const tau_parameter &rhs) 
    {
        return period < rhs.period;
    }/* data */
};

void *tau_process(void* tau_time)
{
    printf("testalive");
	tau_parameter *tau_time_imag = (tau_parameter *) tau_time;
	int workload_time = tau_time_imag -> exectime;
	int period_time = tau_time_imag -> period;
	int priority_num = tau_time_imag -> priority;
	
	printf("period: %d, wordload: %d, priority: %d\n" ,workload_time,period_time,priority_num);
    fflush(stdout);
    setSchedulingPolicy(SCHED_FIFO,priority_num);
    while (1)
    {   
        std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
        for (int j = 0; j < workload_time; j++)
        {
            workload_1ms();
        }
        std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
        const int delta = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        if (delta > period_time)
        {
            printf("Workload smaller and this task period time is %d, and corresponding worst case time is %d\n",period_time,workload_time); 
            fflush(stdout);
            continue;
        }
        else // delta bigger than period, so that process has a big response to pause process.
        {
            printf("This task response time is %d ms, and corresponding worst case time is %d ms\n",(period_time/1000),workload_time); 
            std::cout << "and the workload is " << delta << "ms\n" << std::endl;
            fflush(stdout);
            usleep (period_time-delta);
        }
    }

}

static void rate_monotonic(tau_parameter *priority_sort_array)
{
	sort(priority_sort_array,priority_sort_array + TASK_NUMBER);
	for (size_t i = 0; i < TASK_NUMBER; i++)
	{
		priority_sort_array[i].priority = 99-i;
	}
}


int main(void)
{
    pinCPU(CPU_CORE);
    constexpr int tasknumber = TASK_NUMBER;
    pthread_t tau_threads[tasknumber];
	
    //tau parameter declaration
    tau_parameter tau_time[tasknumber]=
    {
        {WORKLOADTIME_TAU1,PERIODTIME_TAU1,DEFAULT_PRIORITY},
        {WORKLOADTIME_TAU2,PERIODTIME_TAU2,DEFAULT_PRIORITY},
        {WORKLOADTIME_TAU3,PERIODTIME_TAU3,DEFAULT_PRIORITY}
    };    

	//tau prioriry sort by rate_Monotonic 
	rate_monotonic(tau_time);
    printf("now tau_time last is %d",tau_time[2].priority);

  	for (size_t i = 0; i < TASK_NUMBER; ++i)
  	{
  		if (pthread_create(&tau_threads[i], NULL, tau_process, (void *)&tau_time[i]) != 0)
        {
            fflush(stdout);
  		    perror("pthread_create()");
            exit(1);
        }
        

  	}
  
  	for (size_t i = 0; i < TASK_NUMBER; ++i)
  	{

    	if (pthread_join(tau_threads[i], NULL) != 0)
    	perror("pthread_join()");
        exit(1);
  	}
    return 0; 
}


/*

void *do_one_thing(void *pnum_times)
{
  int i, j, x;
  for (i = 0;  i < 4; i++) {
    printf("doing one thing\n"); 
    for (j = 0; j < 10000; j++) x = x + i;
    (*(int *)pnum_times)++;
  }
  return(NULL);
}

void *do_another_thing(void *pnum_times)
{
  int i, j, x;
  for (i = 0;  i < 4; i++) {
    printf("doing another \n"); 
    for (j = 0; j < 10000; j++) x = x + i;
    (*(int *)pnum_times)++;
  }
  return(NULL);
}

void do_wrap_up(int one_times, int another_times)
{
  int total;
  total = one_times + another_times;
  printf("All done, one thing %d, another %d for a total of %d\n",
	one_times, another_times, total);
}
*/

void pinCPU(int cpu_number) {
    cpu_set_t mask;
    CPU_ZERO(&mask);

    CPU_SET(cpu_number, &mask);

    if(sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }
}

void setSchedulingPolicy (int newPolicy, int priority)
{
    sched_param sched;
    int oldPolicy;
    if (pthread_getschedparam(pthread_self(), &oldPolicy, &sched)) {
        perror("pthread_setschedparam");
        exit(EXIT_FAILURE);
    }
    sched.sched_priority = priority;
    if (pthread_setschedparam(pthread_self(), newPolicy, &sched)) {
        perror("pthread_setschedparam");
        exit(EXIT_FAILURE);
    }
}

void workload_1ms (void)
{
	int repeat = 625; // tune this for the right amount of workload
	int x=0;
	for (int i = 0; i <= repeat; i++)
	{
		// add some computation here (e.g., use sqrt() in cmath)
		x = sqrt(2);
	}
}


