# Process Scheduler
A C++ process scheduler project using predefined processes and queue structs 

This Process Scheduler was built and runs on the Linux build of Code::Blocks v20.03 using g++ compiler and 
C++11 ISO C++ language standard compiler flag.

To run this in Code::Blocks, create a New Project -> Console Application -> C++ -> Name the project and select
a location for the project -> Select GNU GCC compiler -> Finish -> remove "main.cpp" from project with right
click menu -> Drop files into the new project folder -> Delete old "main.cpp" file -> Build and Run.

This was a project for Fall 2021 semester in COP4610 Operating Systems class at FAU.

This process scheduler project uses predefined tasks, which were specified by the professor. Each process
is defined as a set of CPU burst times and I/O burst times as follows: `{CPU, IO, CPU, IO, ... , CPU}`. 

Every process must begin and end with a CPU burst.

Notes for all algorithms:

1) All processes finishing CPU bursts from any queue are sent to the back of the IO queue, unless the process is   finished.
2) All algorithms assume unlimited IO devices, so every process in the IO queue is decremented at the same time.
3) Any process in the IO queue is removed from any position in the queue as soon as the IO burst counter reaches 0. This is because of the "unlimited IO devices" ideal condition.

UPDATE: to use this project with GDB (GNU DeBugger), compile the `sched_driver` binary executable:
    
1) Bash: `g++ -g -Wall -Werror *.cpp *.h -o sched_driver`
2) Launch with GDB: `gdb ./sched_driver`

This project provides three different scheduling algorithms, which are selectable by the user:

1) `FCFS`: First Come First Served:
    - This algorithm uses a single Ready Queue and a single IO Queue.
    - Each process in the ready queue is run by the CPU in the order it was received until it completes 
      its CPU burst.
    - This scheduling algorithm is non-preemptive.
2) `SJF`: Shortest Job First:
    - This algorithm uses a single Ready Queue and a single IO Queue.
    - Since each process is predefined, as processes enter the ready queue, they are ordered with the 
        shortest CPU burst in the front of the queue.
    - All processes run until their CPU burst is completed.
    - This scheduling algorithm is non-preemptive.
3) `MLFQ`: Multi-Level Feedback Queue:
    - This algorithm uses three Ready Queues and one IO queue.
    - The Ready Queues use separate priorities and separate algorithms. The highest level Ready Queue uses
        a 5 time quantum (Tq) Round Robin scheme, the second level Ready Queue uses a 10 Tq Round Round scheme, 
        the third Ready Queue uses an FCFS algorithm. 
    - This is a pre-emptive scheduling algorithm.
    - Any process which does not finish its CPU burst in the 1st queue gets moved to the 2nd queue. The 2nd queue
        does not run until the 1st is empty. Any process which does not finish its CPU burst in the 2nd queue gets
        moved to the 3rd queue. The 3rd queue does not run until the 1st and 2nd queues are empty.
    - All processes returning from IO are placed in the 1st queue.
    - Any process running in a lower queue will be preempted by a process arriving in a higher queue, then resume
        its CPU burst when the higher queue(s) are empty.

## FCFS Output Screenshot:
![FCFS Output](./Documentation/Results_FCFS.png)

## SJF Output Screenshot:
![SJF Output](./Documentation/Results_SJF.png)

## MLFQ Output Screenshot:
![MLFQ Output](./Documentation/Results_MLFQ.png)

## Status:

![GitHub repo size](https://img.shields.io/github/repo-size/ADolbyB/process-scheduler?label=Repo%20Size&logo=Github)