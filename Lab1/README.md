# Table Of Content
Dir 的名字都是根據作業主題的方式命名，若要快速找到作業在哪裡，請按照老師的 summary ：
- A file named 1ms.cpp that includes your function for a one-millisecond workload.
    - 放在 Tuning the workload of our task
- A screen shot of top showing the row of %Cpu0 when you concurrently run t1 and t2 on cpu 0.
    - periodic task set and RM scheduling
    - 不過在測試過程中，若把各 process PRI 都拉到99，我的系統多次觸發到 kernal panic....
- The code for tasks t1rm, t2rm, and t3rm, which enable the use of signal SIGINT to get the current deadline miss ratio.
    - Getting RM scheduling statistics
- Files named 'rm.output' and 'rmi.output' for the deadline miss ratios under RM and RMI, and a file named 'cmp.txt' including your observation of the results.
    - Evaluating the timing performance of RM scheduling	
- The figure of t1rm's response time distribution.
    - Plotting the response time
    - 測出來的數據似乎跟老師的版本有落差，似乎跟處理器性質有關係？