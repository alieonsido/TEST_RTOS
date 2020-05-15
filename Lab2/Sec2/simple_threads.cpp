#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <cmath>
#include <unistd.h>

void *do_one_thing(void *);
void *do_another_thing(void *);
void do_wrap_up(int, int);
void pinCPU (int cpu_number);
void setSchedulingPolicy (int newPolicy, int priority);
void workload_1ms (void);

int r1 = 0, r2 = 0;

#define CPU_CORE 0
#define WORKLOADTIME 2000
#define PERIODTIME 7000
#define PRIORITY 98

extern int main(void)
{
  pinCPU(CPU_CORE);
  for (int j = 0; j < WORKLOADTIME; j++)
  {
      workload_1ms ();
  }

  pthread_t thread1, thread2;
  if (pthread_create(&thread1, NULL, do_one_thing, (void *) &r1) != 0)
	perror("pthread_create"), exit(1); 

  if (pthread_create(&thread2, NULL, do_another_thing, (void *) &r2) != 0)
	perror("pthread_create"), exit(1); 
  
  if (pthread_join(thread1, NULL) != 0)
	perror("pthread_join"),exit(1);

  if (pthread_join(thread2, NULL) != 0)
	perror("pthread_join"),exit(1);

  do_wrap_up(r1, r2);

  return 0; 
}

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

void pinCPU (int cpu_number)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);

    CPU_SET(cpu_number, &mask);

    if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1)
    {
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