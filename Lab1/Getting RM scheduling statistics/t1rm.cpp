//#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <unistd.h>
#include <signal.h>

#define CPU_CORE 0
#define WORKLOADTIME 2000
#define PERIODTIME 5000

#define PRIORITY 99

sig_atomic_t executed_times=0;
sig_atomic_t missing_times=0;

void collectData (int param)
{
    float ratio = static_cast<float>(::missing_times) / static_cast<float>(::executed_times);
    std::cout << "Data missing times:" << ::missing_times << std::endl;
    std::cout << "Data executed times:" << ::executed_times << std::endl;
    std::cout << "Data ratio:" << ratio << std::endl;
    std::cout << "\n" << std::endl;
}

void setSchedulingPolicy (int policy, int priority)
{
    sched_param sched;
    sched_getparam(0, &sched);
    sched.sched_priority = priority;
    if (sched_setscheduler(0, policy, &sched)) 
    {
        perror("sched_setscheduler");
        exit(EXIT_FAILURE);
    }
}

void workload_1ms (void)
{
	int repeat = 625; // tune this for the right amount of workload
	int x = 0;
	for (int i = 0; i <= repeat; i++)
	{
		// add some computation here (e.g., use sqrt() in cmath)
		x = sqrt(2);
	}
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

int main (void)
{
    void (*prev_handler)(int);
    prev_handler = signal (SIGINT, collectData);

    // printf("Choose your CPU: ");
    // scanf("%d",&cpu_core);
    pinCPU(CPU_CORE);
    int period = PERIODTIME; // unit: microsecond
    int delta;
    setSchedulingPolicy (SCHED_FIFO, PRIORITY);
    while (1)
    {
        std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
        for (int j = 0; j < WORKLOADTIME; j++)
        {
            workload_1ms ();
        }
        std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
        delta = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        ::executed_times++;
        if (delta > period)
        {
            ::missing_times++;
            continue;
        }
        else
        {
            usleep (period-delta);
        }
    }
    return 0;
}