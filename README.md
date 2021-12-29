# Process_Scheduler
A C++ process scheduler project using predefined processes and queue structs 

This Process Scheduler was built and runs on the Linux Build of Code::Blocks v20.03 using g++ compiler and 
C++11 ISO C++ language standard compiler flag.

This was a project for Fall 2021 semester in COP4610 Operating Systems class at FAU.

This process scheduler project uses predefined tasks, which were specified by the professor. Each process
is defined as a set of CPU burst times and I/O burst times as follows: {CPU, IO, CPU, IO, ... , CPU}. 

Every process must begin and end with a CPU burst.

This project provides three different scheduling algorithms, which are selectable by the user.
1) FCFS: First Come First Served: 
    a) This algorithm uses a single Ready Queue and a single IO Queue.
    b) Each process in the ready queue is run by the CPU in the order it was received until it completes 
        its CPU burst. 
    c) This queue is non-preemptive.
2) SJF: Shortest Job First: 
    a) This algorithm uses a single Ready Queue and a single IO Queue.
    b) Since each process is predefined, as processes enter the ready queue, they are ordered with the 
        shortest CPU burst in the front of the queue. 
    c) All processes run until their CPU burst is completed.
    d) This queue is non-preemptive.
3) MLFQ: Multi-Level Feedback Queue:
    a) This algorithm uses three Ready Queues and one IO queue.
    b) The Ready Queues use separate priorities and separate algorithms. The highest level Ready Queue uses
        a 5 time quantum (Tq) Round Robin scheme, the second level ready queue uses a 10 Tq Round Round scheme, 
        the third Ready queue uses an FCFS algorithm. 
    c) This is a pre-emptive algorithm.
    d) Any process which does not finish its CPU burst in the 1st queue gets moved to the 2nd queue. The 2nd queue
        does not run until the 1st is empty. Any process which does not finish its CPU burst in the 2nd queue gets
        moved to the 3rd queue. The 3rd queue does not run until the 1st and 2nd queues are empty.
    e) All processes returning from IO are placed in the 1st queue.
    f) Any process running in a lower queue will be preempted by a process arriving in a higher queue, then resume
        its CPU burst when the higher queue(s) are empty.
    