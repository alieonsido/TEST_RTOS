#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>

#include <iostream>
#include <chrono>
#include <cmath>

void setSchedulingPolicy (int policy, int priority)
{
    sched_param sched;
    sched_getparam(0, &sched);
    sched.sched_priority = priority;
    if (sched_setscheduler(0, policy, &sched)) {
        perror("sched_setscheduler");
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

int main (void)
{
	setSchedulingPolicy (SCHED_FIFO, 99);
	int delta=5;
	while(delta!=1)
	{
		std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
		workload_1ms ();
		std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
		delta = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
	}
		std::cout << delta << std::endl;

	return 0;
}