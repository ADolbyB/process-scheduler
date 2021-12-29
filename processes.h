/**
Joel Brigida
COP4610-002
Dr. Boijovie Furht
Programming Term Project: Process Scheduler
Description:
    This is the Header File for the Processes and Queues and their nodes.
*/

#ifndef PROCESSES_H
#define PROCESSES_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct node             /** Process & Queue Nodes */
{

    int node_number;                                        // position of node in the list: 1 = Front of list.
    int time_q;                                             // Time amount allocated to each node
    int wait_time;                                          // node must track its own wait time
    char burst_type;                                        // 'C' == CPU burst, 'I' == I/O burst.
    string name;                                            // name of process: P1, P2, P3, P4, etc.
    node *next;                                             // pointers point to next and the previous nodes.

};

struct Process
{
                            /** Functions **/
    Process(string );                                       // default constructor
    ~Process();                                             // destructor
    void Make_process(string, string );                     // makes a queue of nodes by reading a process file
    void add_back(const int&, const string& );              // Add a node to the BACK
    void deQueue();                                         // delete a node from the FRONT
    void Print_proc_nodes(string );                         // iterates the queue and prints the list of nodes.
    bool Empty() { return front == 0; }                     // check if the queue is empty

                            /** Variables **/
    node *front;                                            // points to the front of the queue
    node *back;                                             // points to the back of the queue
    string proc_name;                                       // name of process: P1, P2, P3, P4, etc
    int time_finished;                                      // time process finished: used for T_turnaround;
    int time_response;                                      // time which process 1st started executing
    int total_wait_time;                                    // tracks total wait time of each process
    int nodes_remaining;                                    // running total number of nodes in process
    int total_nodes;                                        // total number of nodes in process (static)

};

struct queue
{
                            /** Functions */
    queue(string, char );                                   // 1st explicit value constructor for FCFS, I/O, and SJF queues
    queue(string, char, int);                               // 2nd explicit value constructor for Round Robin queues
    ~queue();                                               // destructor
    void deQueue();                                         // delete a node from the FRONT
    void remove(node* );                                    // removes a node from the middle of a queue
    void FCFS_SJF_increment_wait();                         // increments the wait times of processes in the ready queue
    void FCFS_SJF_add_to_queue(queue&, Process& );          // ONLY for FCFS and SJF to transfer nodes between queues
    void FCFS_SJF_CPU_queue_timer(queue&, Process&, Process&,
        Process&, Process&, Process&,
        Process&, Process&, Process& );                     // decrements CPU time of front node and increments global clock
    void FCFS_SJF_IO_queue_timer(queue&, Process&, Process&,
        Process&, Process&, Process&,
        Process&, Process&, Process& );
    void MLFQ_add_to_ready1(queue&, Process& );
    void MLFQ_increment_wait(queue&, queue&, queue& );      // increments all wait times of all ready queues for MLFQ
    void MLFQ_add_to_queue(queue&, Process& );              // ONLY for MLFQ to transfer nodes between queues
    void MLFQ_CPU_timer(queue&, queue&, queue&, Process&,
        Process&, Process&, Process&, Process&,
        Process&, Process&, Process& );                     // Function only called for MLFQ Queue timers: Called by Ready_1
    void MLFQ_IO_queue_timer(queue&, Process&, Process&,
        Process&, Process&, Process&,
        Process&, Process&, Process& );                     // decrements I/O time of entire queue. Increments total IO clock
    void Print_ready_IO(string );                           // iterates the queue and prints the list of nodes.
    void Results(Process&, Process&, Process&,
        Process&, Process&, Process&, Process&, Process& ); // calculates and prints results: Ave T_wait, Ave T_turnaround, Ave T_response
    bool Empty() { return front == 0; }                     // check if the queue is empty

                            /** Variables */
    node *front;                                            // points to the front of the queue
    node *back;                                             // points to the back of the queue
    string queue_name;                                      // name of process: P1, P2, P3, P4, etc.
    char algorithm;                                         // algorithm running in queue: 'F' = FCFS, 'S' = SJF, 'R' = Round Robin
    int nodes_remaining;                                    // running total number of nodes in process
    int total_nodes;                                        // total # of nodes processed by the queue
    int CPU_total_time;                                     // total time spent doing CPU bursts
    int IO_total_time;                                      // total time spent doing IO bursts
    int CPU_idle_time;                                      // total time CPU is idle.
    int Global_clock;                                       // overall Global time for all processes
    int RR_time;                                            // T_q for each Round Robin queue

};

#endif
