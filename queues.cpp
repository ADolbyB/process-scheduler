/**
Joel Brigida
COP4610-002
Dr. Boijovie Furht
Programming Term Project: Process Scheduler
Description:
    This Implementation file forms the CPU/Ready Queue and the I/O queue
    The Processes are formed in the processes implementation file
*/

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "processes.h"

using namespace std;

queue::queue(string the_name, char letter)                  // initialize an empty queue FCFS and SJF Queues ONLY
{
    //cout << "1st EV Constructor Called" << endl;

    front = back = 0;                                       // set front and back pointers to NULL
    queue_name = the_name;                                  // initialize process name
    nodes_remaining = 0;                                    // set nodes_remaining to 0
    total_nodes = 0;
    algorithm = letter;                                     // 'F' = FCFS, 'S' = SJF, 'M' = MLFQ, 'I' = I/O
    CPU_total_time = 0;                                     // set CPU time to 0
    CPU_idle_time = 0;                                      // total time CPU is idle.
    IO_total_time = 0;                                      // set IO time to 0
    Global_clock = 0;                                       // overall Global time for all processes
    RR_time = 0;                                            // not used in this constructor

}

queue::queue(string the_name, char letter, int time)        // Used for for Round Robin queues ONLY
{
    //cout << "2nd EV Constructor Called" << endl;

    front = back = 0;                                       // set front and back pointers to NULL
    queue_name = the_name;                                  // initialize process name
    nodes_remaining = 0;                                    // set nodes_remaining to 0
    total_nodes = 0;
    algorithm = letter;                                     // 'F' = FCFS, 'S' = SJF, 'M' = MLFQ, 'I' = I/O
    CPU_total_time = 0;                                     // set CPU time to 0
    CPU_idle_time = 0;                                      // total time CPU is idle.
    IO_total_time = 0;                                      // set IO time to 0
    Global_clock = 0;                                       // overall Global time for all processes
    RR_time = time;                                         // ONLY FOR ROUND ROBIN


}

queue::~queue()
{
	//cout << "* Destructor Called *" << endl;
	while (!Empty())
        {
            deQueue();                                      // delete nodes until the list is empty
        }
}

void queue::deQueue()                                       // deletes a node from the FRONT
{
	if (!Empty())
        {
            cout << "Line 67 .deQueue() " << " Node: " << front->name << " | Burst Type: "
                 << front->burst_type << " | Time_q: " << front->time_q << endl;

            node *p = front;                                // declare pointer to point to the front node.

            front = front->next;                            // move the front pointer to the next one down the queue

            if (front == 0)
                {
                    back = 0;                               // if front is NULL after moving pointer, then there are no nodes left in the queue,
                }                                           // so set the back pointer to NULL also (as a precaution)
            delete p;                                       // delete unwanted node.
            nodes_remaining--;                              // decrement the # of nodes remaining in queue
        }
	else
        {
            cout << "Cannot deQueue because queue is empty" << endl;
        }
}

void queue::remove(node* p)
{
	if (p == 0)                                             // if pointer p not found in the list (SHOULD NOT HAPPEN)
        {
            cout << "ERROR: Can't remove a NULL node" << endl;
        }
	else // if (p != 0)                                     // if "p" points to a valid node
        {
            cout << "Line 95 .remove() " << " | Node: " << front->name
                 << " | Burst Type: " << front->burst_type << " | Time_q: " << front->time_q << endl;

            if (p == front && front == back)                // if there is only one node in the list
                {
                    delete p;                               // delete that only node
                    nodes_remaining--;
                    front = back = 0;                       // set both pointers to null since list is now empty
                }
            else if (p == front)                            // if p == the front node: there are at least 2 nodes in the list
                {
                    front = front->next;                    // move front pointer to the next node
                    delete p;                               // delete front node
                    nodes_remaining--;                      // decrement # nodes remaining in the queue
                }
            else                                            // p != front or back, there are at least 3 nodes, and unwanted node is in the middle
                {
                    node *back_ptr = front;                 // declare a new pointer to traverse through the list

                    while (back_ptr != 0 && back_ptr->next != p)        // valid until back->next == p
                        {
                            back_ptr = back_ptr->next;                  // traverse through the list to find node before *p
                        }
                    if (p == back)                                      // if p == back node
                        {
                            back = back_ptr;                            // set back to node before last node
                        }
                    back_ptr->next = p->next;                           // set the "next" pointer to skip the unwanted node
                    delete p;                                           // delete unwanted node.
                    nodes_remaining--;                                  // keeps count of the # of nodes that exist;
                }
        }
}

void queue::FCFS_SJF_increment_wait(void)                               // must be called by READY queue
{
    cout << "Increment_wait() called" << endl;

    node *p = front;

    if (p != 0)
        {
            p = p->next;                                                // skip incrementing wait time for running process in the front.

            if (p == 0)
                {
                    cout << "No Waiting Processes behind Front: No wait times to increment" << endl;
                }

            while (p != 0)
                {
                    p->wait_time++;                                     // increment wait time of node BEHIND front
                    p = p->next;                                        // traverse the queue
                }
        }
    else // if (p == 0)
        {
            cout << "Ready Queue Empty: No wait times to increment" << endl;
        }
}

void queue::FCFS_SJF_add_to_queue(queue& other_queue, Process& the_obj)
{
    if (this->queue_name == "Ready")
        {
            cout << "Adding process to the Ready List" << endl;
        }
    else // if (this->queue_name == "IO_wait")
        {
            cout << "Transferring Process to I/O queue" << endl;
            cout << "Adding wait time to total wait time" << endl;
            cout << "Wait time for this CPU burst: " << other_queue.front->wait_time << endl;
            the_obj.total_wait_time += other_queue.front->wait_time;                    // take node wait time and add to total wait time of process
            cout << "Updated Wait Time: " << the_obj.total_wait_time << endl;
        }

    if (the_obj.Empty())
        {
            cout << "Nothing to add to queue: Process is finished" << endl;
            cout << "Storing time_finished" << endl;
            the_obj.time_finished = other_queue.Global_clock;
            cout << "time_finished = " << the_obj.time_finished << endl;
        }
    else // if NOT empty
        {
            int burst_time;
            burst_time = the_obj.front->time_q;

            char burst_type;
            burst_type = the_obj.front->burst_type;

            string name;
            name = the_obj.front->name;
            cout << "*** Adding node | name: " << name << " | burst_type: " << burst_type << " | burst_time: " << burst_time << endl;

            if (this->algorithm == 'F' || this->algorithm == 'I')           // only valid for I/O queue and FCFS Ready queue algorithm
                {
                    if (Empty())
                        {
                            front = back = new node;                        // declare a new node, set front and back to point to the same new node.
                            nodes_remaining++;                              // increment the # of nodes remaining in the queue
                            total_nodes++;                                  // increment total # of nodes created
                            front->time_q = burst_time;                     // add the time value to the new node
                            front->wait_time = 0;                           // initialize wait time of node to ZERO
                            front->node_number = 1;                         // 1st node is always = 1
                            front->burst_type = burst_type;                 // set burst type for new queue node
                            front->name = name;                             // make sure to name the node so we know where it came from
                            back->next = 0;
                        }
                    else // if not empty
                        {
                            back->next = new node;                          // declare a new node after the current back node. (default constructor makes *back & *front of new node = NULL already
                            nodes_remaining++;                              // increment the # of nodes remaining in the queue
                            total_nodes++;                                  // increment total # of nodes created
                            back = back->next;                              // move the back pointer to the new node, which is now in the back
                            back->time_q = burst_time;                      // add the time value to the new node
                            back->wait_time = 0;
                            back->node_number = total_nodes;                // new node takes position in the queue as the total # of nodes.
                            back->burst_type = burst_type;
                            back->name = name;                              // make sure to name the node so we know where it came from
                            back->next = 0;                                 // set the "next" field of the back node to NULL
                        }
                }
            else if (this->algorithm == 'S')                                // Valid only for SJF Ready Queue
                {
                    if (Empty())
                        {
                            front = back = new node;                        // declare a new node, set front and back to point to the same new node.
                            nodes_remaining++;                              // increment the # of nodes remaining in the queue
                            total_nodes++;                                  // increment total # of nodes created
                            front->time_q = burst_time;                     // add the time value to the new node
                            front->wait_time = 0;                           // initialize wait time of node to ZERO
                            front->node_number = 1;                         // 1st node is always = 1
                            front->burst_type = burst_type;                 // set burst type for new queue node
                            front->name = name;                             // make sure to name the node so we know where it came from
                            back->next = 0;
                        }
                    else // if NOT empty
                        {
                            node *p = front;                                // declare node to point to the front of the queue

                            if (p != 0)
                                {
                                    p = p->next;                            // p->next only exists if the front node is NOT NULL.
                                }

                            while (p != 0 && (the_obj.front->time_q >= p->time_q))
                                {
                                    p = p->next;                            // keep traversing and looking for a lower time
                                }

                            if (p == 0)                                     // if *p is NULL, then it got to the end of the list and found nothing
                                {
                                    back->next = new node;                  // create a new node at the end of the list (node w/ greatest time)
                                    nodes_remaining++;
                                    total_nodes++;
                                    back = back->next;                      // move back pointer
                                    back->time_q = burst_time;              // add the time value to the new node
                                    back->wait_time = 0;
                                    back->node_number = total_nodes;        // new node takes position in the queue as the total # of nodes.
                                    back->burst_type = burst_type;
                                    back->name = name;                      // make sure to name the node so we know where it came from
                                    back->next = 0;                         // set the "next" field of the back node to NULL
                                }
                            else    // if *p != NULL, then it stopped at a node
                                {
                                    node *back_ptr = front;

                                    while (back_ptr != 0 && back_ptr->next != p)
                                        {
                                            back_ptr = back_ptr->next;
                                        }
                                    node *q = new node;                     // create new node to be inserted.
                                    nodes_remaining++;
                                    total_nodes++;
                                    q->time_q = burst_time;                 // add the time value to the new node
                                    q->wait_time = 0;
                                    q->node_number = total_nodes;           // new node takes position in the queue as the total # of nodes.
                                    q->burst_type = burst_type;
                                    q->name = name;                         // make sure to name the node so we know where it came from
                                    q->next = p;                            // q will be inbetween p and p->next
                                    back_ptr->next = q;                     // point p->next to q. Now order is: p => q => q->next
                                }
                        }
                }
            cout << "Line 280: .deQueue() called by: " << the_obj.proc_name << " | Node: " << the_obj.front->name
                 << " | Burst Type: " << the_obj.front->burst_type << " | Time_q: " << the_obj.front->time_q << endl;
            the_obj.deQueue();                                              // delete the front proc_node from the process since it's now in the queue.
        }
}

void queue::FCFS_SJF_CPU_queue_timer(queue& IO, Process& obj1, Process& obj2,        // calling object is the READY queue
    Process& obj3, Process& obj4, Process& obj5, Process& obj6, Process& obj7, Process& obj8)
{
    cout << "Calling Ready_Queue_Timer" << endl;

    if (!Empty())
        {
            if (front->burst_type == 'C' || front->burst_type == 'c')
                {
                    if (front->time_q > 0)                                  // if there is still CPU time left, decrement the counter
                        {
                            front->time_q--;
                            CPU_total_time++;                               // increment CPU busy time
                            Global_clock++;                                 // increment the global clock
                            IO.FCFS_SJF_IO_queue_timer(*this, obj1, obj2, obj3,
                                obj4, obj5, obj6, obj7, obj8);              // I/O time must be synced with global clock time.
                            FCFS_SJF_increment_wait();                               // increment wait time of nodes in the ready queue
                        }
                    else if (front->time_q == 0)                            // if there is no time left, then CPU burst is done. Delete node from the queue.
                        {
                            cout << "*** ADD TO IO!! ***" << endl;
                            if (front->name == "P1")
                                {
                                    cout << "front->name == P1" << endl;
                                    IO.FCFS_SJF_add_to_queue(*this, obj1);
                                }
                            if (front->name == "P2")
                                {
                                    cout << "front->name == P2" << endl;
                                    if (obj2.time_response == 0)
                                        {
                                            obj2.time_response = front->wait_time;      // update P2 response time
                                        }
                                    IO.FCFS_SJF_add_to_queue(*this, obj2);
                                }
                            if (front->name == "P3")
                                {
                                    cout << "front->name == P3" << endl;
                                    if (obj3.time_response == 0)
                                        {
                                            obj3.time_response = front->wait_time;      // update P3 response time
                                        }
                                    IO.FCFS_SJF_add_to_queue(*this, obj3);
                                }
                            if (front->name == "P4")
                                {   cout << "front->name == P4" << endl;
                                    if (obj4.time_response == 0)
                                        {
                                            obj4.time_response = front->wait_time;      // update P4 response time
                                        }
                                    IO.FCFS_SJF_add_to_queue(*this, obj4);
                                }
                            if (front->name == "P5")
                                {
                                    cout << "front->name == P5" << endl;
                                    if (obj5.time_response == 0)
                                        {
                                            obj5.time_response = front->wait_time;      // update P5 response time
                                        }
                                    IO.FCFS_SJF_add_to_queue(*this, obj5);
                                }
                            if (front->name == "P6")
                                {
                                    cout << "front->name == P6" << endl;
                                    if (obj6.time_response == 0)
                                        {
                                            obj6.time_response = front->wait_time;      // update P6 response time
                                        }
                                    IO.FCFS_SJF_add_to_queue(*this, obj6);
                                }
                            if (front->name == "P7")
                                {
                                    cout << "front->name == P7" << endl;
                                    if (obj7.time_response == 0)
                                        {
                                            obj7.time_response = front->wait_time;      // update P7 response time
                                        }
                                    IO.FCFS_SJF_add_to_queue(*this, obj7);
                                }
                            if (front->name == "P8")
                                {
                                    cout << "front->name == P8" << endl;
                                    if (obj8.time_response == 0)
                                        {
                                            obj8.time_response = front->wait_time;      // update P8 response time
                                        }
                                    IO.FCFS_SJF_add_to_queue(*this, obj8);
                                }
                            cout << "Line 383: calling .dequeue() Queue " << this->queue_name << " | Burst Type: "
                                 << this->front->burst_type << " | Time_q: " << this->front->time_q << endl;
                            deQueue();
                        }
                    else
                        {
                            cout << "CPU_queue_timer: Invalid Condition" << endl;       // debug statement
                        }
                }
        }
    else // if queue is empty
        {
            cout << "Ready Queue Empty: Increment CPU_idle_time" << endl;               // debug
            Global_clock++;
            CPU_idle_time++;
            IO.FCFS_SJF_IO_queue_timer(*this, obj1, obj2, obj3,
                obj4, obj5, obj6, obj7, obj8);                                          // I/O timer must be synced with Global clock.
        }
}

void queue::FCFS_SJF_IO_queue_timer(queue& CPU, Process& obj1, Process& obj2,           // calling object is the IO_wait queue
    Process& obj3, Process& obj4, Process& obj5, Process& obj6, Process& obj7, Process& obj8)
{
    cout << "Calling IO_Queue_Timer" << endl;

    node *p = this->front;                                                              // declare a pointer at the front of the IO_wait queue

    if (p != 0)                                                                         // if the pointer is NULL, then the queue is empty
        {
            if (p->burst_type == 'C' || p->burst_type == 'c')
                {
                    cout << "ERROR: Only Decrement I/O bursts in I/O queue" << endl;    // debug: Should NEVER happen
                    return;
                }
            else // if(!Empty())
                {
                    while (p != 0)
                        {
                            if (p->time_q > 0)
                                {
                                    p->time_q--;                                        // decrement the time in I/O
                                    p = p->next;                                        // traverse the entire I/O list
                                }
                            else // if (p->time_q == 0)
                                {
                                    cout << "*** Add " << p->name << " to ready list ***" << endl;
                                    if (p->name == "P1")
                                        {
                                            cout << "p->name == P1" << endl;
                                            CPU.FCFS_SJF_add_to_queue(*this, obj1);
                                        }
                                    if (p->name == "P2")
                                        {
                                            cout << "p->name == P2" << endl;
                                            CPU.FCFS_SJF_add_to_queue(*this, obj2);
                                        }
                                    if (p->name == "P3")
                                        {
                                            cout << "p->name == P3" << endl;
                                            CPU.FCFS_SJF_add_to_queue(*this, obj3);
                                        }
                                    if (p->name == "P4")
                                        {
                                            cout << "p->name == P4" << endl;
                                            CPU.FCFS_SJF_add_to_queue(*this, obj4);
                                        }
                                    if (p->name == "P5")
                                        {
                                            cout << "p->name == P5" << endl;
                                            CPU.FCFS_SJF_add_to_queue(*this, obj5);
                                        }
                                    if (p->name == "P6")
                                        {
                                            cout << "p->name == P6" << endl;
                                            CPU.FCFS_SJF_add_to_queue(*this, obj6);
                                        }
                                    if (p->name == "P7")
                                        {
                                            cout << "p->name == P7" << endl;
                                            CPU.FCFS_SJF_add_to_queue(*this, obj7);
                                        }
                                    if (p->name == "P8")
                                        {
                                            cout << "p->name == P8" << endl;
                                            CPU.FCFS_SJF_add_to_queue(*this, obj8);
                                        }
                                    cout << "*** Remove from I/O ***" << endl;
                                    remove(p);                                              // delete the node from I/O queue if its finished I/O
                                    p = p->next;                                            // continue traversing the list
                                }
                        }
                    IO_total_time++;
                }
        }
    else
        {
            cout << "IO_Queue empty: Don't adjust IO_total_time" << endl;
        }
}

void queue::MLFQ_add_to_ready1(queue& other_queue, Process& the_obj)    // ONLY for initial loading of the Processes to the Ready List
{
    cout << "Loading initial process into Ready_1 Queue before Execution" << endl;

    if (the_obj.Empty())
        {
            cout << "Nothing to add to queue!!" << endl;
        }
    else
        {
            int burst_time;
            burst_time = the_obj.front->time_q;

            char burst_type;
            burst_type = the_obj.front->burst_type;

            string name;
            name = the_obj.front->name;
            cout << "*** Adding node | name: " << name << " | burst_type: " << burst_type << " | burst_time: " << burst_time << endl;

            if (Empty())
                {
                    front = back = new node;                        // declare a new node, set front and back to point to the same new node.
                    nodes_remaining++;                              // increment the # of nodes remaining in the queue
                    total_nodes++;
                    front->time_q = burst_time;                     // add the time value to the new node
                    front->wait_time = 0;                           // initialize wait time of node to ZERO
                    front->node_number = 1;                         // 1st node is always = 1
                    front->burst_type = burst_type;                 // set burst type for new queue node
                    front->name = name;                             // make sure to name the node so we know where it came from
                    back->next = 0;
                }
            else // if not empty
                {
                    back->next = new node;                          // declare a new node after the current back node. (default constructor makes *back & *front of new node = NULL already
                    nodes_remaining++;                              // increment the # of nodes remaining in the queue
                    total_nodes++;
                    back = back->next;                              // move the back pointer to the new node, which is now in the back
                    back->time_q = burst_time;                      // add the time value to the new node
                    back->wait_time = 0;
                    back->node_number = total_nodes;                // new node takes position in the queue as the total # of nodes.
                    back->burst_type = burst_type;
                    back->name = name;                              // make sure to name the node so we know where it came from
                    back->next = 0;                                 // set the "next" field of the back node to NULL
                }
            the_obj.deQueue();                                      // delete the front proc_node from the process since it's now in the queue.
        }
}

void queue::MLFQ_increment_wait(queue& RR1a, queue& RR2a, queue& FCa)
{
    cout << "MLFQ_Increment_wait() called" << endl;

    node *p = RR1a.front;

    if (p != 0)
        {
            p = p->next;                                            // 1st node is always running if there is one in the list

            while (p != 0)
                {
                    p->wait_time++;                                 // increment wait time of node BEHIND front
                    p = p->next;                                    // traverse the queue
                }
        }
    else // if (p != 0)
        {
            cout << "Ready_1 queue front == 0: no wait times to increment" << endl;
        }

    node *q = RR2a.front;

    if (q != 0)
        {
             if (RR1a.Empty())
                {
                    q = q->next;                        // EXCEPTION: If Ready_1 is empty, Ready_2 is running: skip the 1st node wait time
                }

            while (q != 0)
                {
                    q->wait_time++;                     // increment wait time of node BEHIND front
                    q = q->next;                        // traverse the queue
                }
        }
    else // if (p != 0)
        {
            cout << "Ready_2 queue front == 0: no wait times to increment" << endl;
        }

    node *r = FCa.front;

    if (r != 0)
        {
            if (RR1a.Empty() && RR2a.Empty())
                {
                    r = r->next;                        // EXCEPTION: the front node is not waiting if the other 2 queues are running
                }

            while (r != 0)
                {
                    r->wait_time++;                     // increment wait time of node BEHIND front
                    r = r->next;                        // traverse the queue
                }
        }
    else // if (p != 0)
        {
            cout << "Ready_3 FCFS queue front == 0: no wait times to increment" << endl;
        }
}

void queue::MLFQ_add_to_queue(queue& other_queue, Process& the_obj)                     // can be called by I/O or any Ready Queue
{
    if (this->queue_name == "Ready_1")
        {
            cout << "Adding Process to Ready_1 List (RR-5)" << endl;
        }
    else if (this->queue_name == "Ready_2")
        {
            cout << "Transferring Process to Ready_2 List (RR-10)" << endl;
            cout << "Adding wait time to total wait time" << endl;
            cout << "Wait time for this CPU burst: " << other_queue.front->wait_time << endl;

            the_obj.total_wait_time += other_queue.front->wait_time;// take node wait time and add to total wait time of process

            cout << "Updated Wait Time: " << the_obj.total_wait_time << endl;
        }
    else if (this->queue_name == "Ready_3")
        {
            cout << "Transferring Process to Ready_3 List (FCFS)" << endl;
            cout << "Adding wait time to total wait time" << endl;
            cout << "Wait time for this CPU burst: " << other_queue.front->wait_time << endl;

            the_obj.total_wait_time += other_queue.front->wait_time;// take node wait time and add to total wait time of process

            cout << "Updated Wait Time: " << the_obj.total_wait_time << endl;
        }
    else // if (this->queue_name == "IO_wait")
        {
            cout << "Transferring Process to I/O queue" << endl;
            cout << "Adding wait time to total wait time" << endl;
            cout << "Wait time for this CPU burst: " << other_queue.front->wait_time << endl;

            the_obj.total_wait_time += other_queue.front->wait_time;// take node wait time and add to total wait time of process

            cout << "Updated Wait Time: " << the_obj.total_wait_time << endl;
        }

    if (the_obj.Empty())
        {
            cout << "Nothing to add to queue: Process is finished" << endl;
            cout << "Storing time_finished" << endl;
            the_obj.time_finished = other_queue.Global_clock;
            cout << "time_finished = " << the_obj.time_finished << endl;
        }
    else
        {
            if (this->queue_name == "IO_wait")                      // If we need to Add to I/O queue
                {
                    //cout << "Line 633: Inside IF" << endl;
                    //cout << "Line 634: this->queue_name == " << this->queue_name << endl;

                    int burst_time;
                    burst_time = the_obj.front->time_q;             // grab data from FRONT of process to be copied to I/O queue node

                    char burst_type;
                    burst_type = the_obj.front->burst_type;

                    string name;
                    name = the_obj.front->name;
                    cout << "*** Adding node to I/O | name: " << name << " | burst_type: " << burst_type
                         << " | burst_time: " << burst_time << endl;

                    if (Empty())    // add node to I/O here
                        {
                            front = back = new node;                // declare a new node, set front and back to point to the same new node.
                            nodes_remaining++;                      // increment the # of nodes remaining in the queue
                            total_nodes++;                          // increment total # of nodes created
                            front->time_q = burst_time;             // add the time value to the new node
                            front->wait_time = 0;                   // initialize wait time of node to ZERO
                            front->node_number = 1;                 // 1st node is always = 1
                            front->burst_type = burst_type;         // set burst type for new queue node
                            front->name = name;                     // make sure to name the node so we know where it came from
                            back->next = 0;
                        }
                    else // if not empty: add node to I/O here
                        {
                            back->next = new node;                  // declare a new node after the current back node. (default constructor makes *back & *front of new node = NULL already
                            nodes_remaining++;                      // increment the # of nodes remaining in the queue
                            total_nodes++;                          // increment total # of nodes created
                            back = back->next;                      // move the back pointer to the new node, which is now in the back
                            back->time_q = burst_time;              // add the time value to the new node
                            back->wait_time = 0;
                            back->node_number = total_nodes;        // new node takes position in the queue as the total # of nodes.
                            back->burst_type = burst_type;
                            back->name = name;                      // make sure to name the node so we know where it came from
                            back->next = 0;                         // set the "next" field of the back node to NULL
                        }
                    cout << "Line 658: Obj calling .deQueue(): " << the_obj.proc_name << " | Node Name: " << the_obj.front->name
                         << " | Burst Type: " << the_obj.front->burst_type << " | Time_q: " << the_obj.front->time_q << endl;

                    the_obj.deQueue();                              // delete the front PROCESS node from process since it's now in the queue.
                }   // END if (this->queue_name == "IO_wait")

            else if (this->queue_name == "Ready_1")                 // Ready_1 Calling: Add to Ready_1 from I/O queue
                {
                    //cout << "Line 680: Inside ELSE-IF" << endl;
                    //cout << "Line 681: this->queue_name == " << this->queue_name << endl;

                    int burst_time;
                    burst_time = the_obj.front->time_q;

                    char burst_type;
                    burst_type = the_obj.front->burst_type;

                    string name;
                    name = the_obj.front->name;

                    cout << "*** Adding node to " << this->queue_name << " | name: " << name << " | burst_type: "
                         << burst_type << " | burst_time: " << burst_time << endl;

                    if (Empty())                                    // if the Ready_1 queue is empty
                        {
                            front = back = new node;                // declare a new node, set front and back to point to the same new node.
                            nodes_remaining++;                      // increment the # of nodes remaining in the queue
                            total_nodes++;                          // increment total # of nodes created
                            front->time_q = burst_time;             // add the time value to the new node
                            front->wait_time = 0;                   // initialize wait time of node to ZERO
                            front->node_number = 1;                 // 1st node is always = 1
                            front->burst_type = burst_type;         // set burst type for new queue node
                            front->name = name;                     // make sure to name the node so we know where it came from
                            back->next = 0;
                        }
                    else // if NOT empty : Add a node to the BACK of the Ready_1 Queue
                        {
                            back->next = new node;                  // create a new node at the end of the list (node w/ greatest time)
                            nodes_remaining++;
                            total_nodes++;
                            back = back->next;                      // move back pointer
                            back->time_q = burst_time;              // add the time value to the new node
                            back->wait_time = 0;
                            back->node_number = total_nodes;        // new node takes position in the queue as the total # of nodes.
                            back->burst_type = burst_type;
                            back->name = name;                      // make sure to name the node so we know where it came from
                            back->next = 0;                         // set the "next" field of the back node to NULL
                        }
                }

            else if (this->queue_name == "Ready_2" || "Ready_3")                    // ONLY for MLFQ algo: Ready_2 and Ready_3 calling
                {                                                                   // Ready_2 or Ready_3 Call if moving FROM Ready_1 or Ready_2
                    //cout << "Line 724: Inside Else" << endl;
                    //cout << "Line 725: this->queue_name == " << this->queue_name << endl;

                    int burst_time;
                    char burst_type;
                    string name;

                    if (other_queue.front->time_q > 0)                              // If there is still CPU Burst time left
                        {
                            burst_time = other_queue.front->time_q;                 // use the CPU time left at the front node of Higher Priority Queue
                            burst_type = other_queue.front->burst_type;
                            name = other_queue.front->name;
                        }
                    else                                                            /// is this even a valid case???
                        {
                            burst_time = the_obj.front->time_q;                     // use the CPU burst time at the front of the Process Object
                            burst_type = the_obj.front->burst_type;
                            name = the_obj.front->name;
                        }

                    cout << "*** Adding node to " << this->queue_name << " | name: " << name << " | burst_type: "
                         << burst_type << " | burst_time: " << burst_time << endl;

                    if (Empty())
                        {
                            front = back = new node;                // declare a new node, set front and back to point to the same new node.
                            nodes_remaining++;                      // increment the # of nodes remaining in the queue
                            total_nodes++;                          // increment total # of nodes created
                            front->time_q = burst_time;             // add the time value to the new node
                            front->wait_time = 0;                   // initialize wait time of node to ZERO
                            front->node_number = 1;                 // 1st node is always = 1
                            front->burst_type = burst_type;         // set burst type for new queue node
                            front->name = name;                     // make sure to name the node so we know where it came from
                            back->next = 0;
                        }
                    else // if not empty
                        {
                            back->next = new node;                  // declare a new node after the current back node. (default constructor makes *back & *front of new node = NULL already
                            nodes_remaining++;                      // increment the # of nodes remaining in the queue
                            total_nodes++;                          // increment total # of nodes created
                            back = back->next;                      // move the back pointer to the new node, which is now in the back
                            back->time_q = burst_time;              // add the time value to the new node
                            back->wait_time = 0;
                            back->node_number = total_nodes;        // new node takes position in the queue as the total # of nodes.
                            back->burst_type = burst_type;
                            back->name = name;                      // make sure to name the node so we know where it came from
                            back->next = 0;                         // set the "next" field of the back node to NULL
                        }
                }   // END else if (this->queue_name == "Ready_2" || "Ready_3")
        } // END else if !(the_obj.Empty())
}

void queue::MLFQ_CPU_timer(queue& IO, queue& RR2, queue& FC, Process& obj1, Process& obj2,  // Calling object is READY_1 ALWAYS
    Process& obj3, Process& obj4, Process& obj5, Process& obj6, Process& obj7, Process& obj8)
{
    cout << "Calling MLFQ_CPU_Timer" << endl;

    if (!Empty()) // && RR2.RR_time == 10)                          // if the first Ready queue is not empty...run the front process
        {
            if (front->burst_type == 'C' || front->burst_type == 'c')
                {
                    if (this->RR_time > 0)
                        {
                            if (front->time_q > 0)
                                {
                                    this->RR_time--;                                        // decrement the Round Robin Time
                                    front->time_q--;                                        // decrement the CPU busrt time of process
                                    CPU_total_time++;                                       // Increment the CPU total time
                                    this->Global_clock++;                                   // Increment Global Clock
                                    RR2.Global_clock++;
                                    FC.Global_clock++;
                                    IO.MLFQ_IO_queue_timer(*this, obj1, obj2, obj3,
                                        obj4, obj5, obj6, obj7, obj8);                      // I/O time must be synced with global clock time.
                                    MLFQ_increment_wait(*this, RR2, FC);                    // Increment wait times
                                }
                            else // if (front->time_q == 0)                                 // send the process to I/O anyway (RR_time expired)
                                {
                                    cout << "*** ADD TO IO!! ***" << endl;
                                    cout << "*** RESET RR timer ***" << endl;
                                    this->RR_time = 5;

                                    if (front->name == "P1")
                                        {
                                            cout << "front->name == P1" << endl;
                                            IO.MLFQ_add_to_queue(*this, obj1);
                                        }
                                    if (front->name == "P2")
                                        {
                                            cout << "front->name == P2" << endl;
                                            if (obj2.time_response == 0)
                                                {
                                                    obj2.time_response = front->wait_time;      // update P2 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj2);
                                        }
                                    if (front->name == "P3")
                                        {
                                            cout << "front->name == P3" << endl;
                                            if (obj3.time_response == 0)
                                                {
                                                    obj3.time_response = front->wait_time;      // update P3 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj3);
                                        }
                                    if (front->name == "P4")
                                        {
                                            cout << "front->name == P4" << endl;
                                            if (obj4.time_response == 0)
                                                {
                                                    obj4.time_response = front->wait_time;      // update P4 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj4);
                                        }
                                    if (front->name == "P5")
                                        {
                                            cout << "front->name == P5" << endl;
                                            if (obj5.time_response == 0)
                                                {
                                                    obj5.time_response = front->wait_time;      // update P5 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj5);
                                        }
                                    if (front->name == "P6")
                                        {
                                            cout << "front->name == P6" << endl;
                                            if (obj6.time_response == 0)
                                                {
                                                    obj6.time_response = front->wait_time;      // update P6 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj6);
                                        }
                                    if (front->name == "P7")
                                        {
                                            cout << "front->name == P7" << endl;
                                            if (obj7.time_response == 0)
                                                {
                                                    obj7.time_response = front->wait_time;      // update P7 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj7);
                                        }
                                    if (front->name == "P8")
                                        {
                                            cout << "front->name == P8" << endl;
                                            if (obj8.time_response == 0)
                                                {
                                                    obj8.time_response = front->wait_time;      // update P8 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj8);
                                        }
                                    this->deQueue();                                            // remove node from READY_1 queue
                                }   // END else // if (front->time_q == 0)
                        }   // END if (this->RR_time > 0)
                    else // if this->RR_time == 0)
                        {
                            if (front->time_q == 0)
                                {
                                    cout << "*** ADD TO IO!! ***" << endl;
                                    cout << "*** RESET RR timer ***" << endl;
                                    this->RR_time = 5;

                                    if (front->name == "P1")
                                        {
                                            cout << "front->name == P1" << endl;
                                            IO.MLFQ_add_to_queue(*this, obj1);
                                        }
                                    if (front->name == "P2")
                                        {
                                            cout << "front->name == P2" << endl;
                                            if (obj2.time_response == 0)
                                                {
                                                    obj2.time_response = front->wait_time;      // update P2 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj2);
                                        }
                                    if (front->name == "P3")
                                        {
                                            cout << "front->name == P3" << endl;
                                            if (obj3.time_response == 0)
                                                {
                                                    obj3.time_response = front->wait_time;      // update P3 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj3);
                                        }
                                    if (front->name == "P4")
                                        {
                                            cout << "front->name == P4" << endl;
                                            if (obj4.time_response == 0)
                                                {
                                                    obj4.time_response = front->wait_time;      // update P4 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj4);
                                        }
                                    if (front->name == "P5")
                                        {
                                            cout << "front->name == P5" << endl;
                                            if (obj5.time_response == 0)
                                                {
                                                    obj5.time_response = front->wait_time;      // update P5 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj5);
                                        }
                                    if (front->name == "P6")
                                        {
                                            cout << "front->name == P6" << endl;
                                            if (obj6.time_response == 0)
                                                {
                                                    obj6.time_response = front->wait_time;      // update P6 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj6);
                                        }
                                    if (front->name == "P7")
                                        {
                                            cout << "front->name == P7" << endl;
                                            if (obj7.time_response == 0)
                                                {
                                                    obj7.time_response = front->wait_time;      // update P7 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj7);
                                        }
                                    if (front->name == "P8")
                                        {
                                            cout << "front->name == P8" << endl;
                                            if (obj8.time_response == 0)
                                                {
                                                    obj8.time_response = front->wait_time;      // update P8 response time
                                                }
                                            IO.MLFQ_add_to_queue(*this, obj8);
                                        }
                                    this->deQueue();                                            // remove node from READY_1 queue
                                }
                            else // if (front->time_q > 0)
                                {
                                    cout << "*** ADD TO Ready_2!! ***" << endl;
                                    cout << "*** Reset RR Timer ***" << endl;
                                    this->RR_time = 5;                                          // Reset RR_time back to 5 for READY_1

                                    if (front->name == "P1")
                                        {
                                            cout << "front->name == P1" << endl;
                                            RR2.MLFQ_add_to_queue(*this, obj1);
                                        }
                                    if (front->name == "P2")
                                        {
                                            cout << "front->name == P2" << endl;
                                            if (obj2.time_response == 0)
                                                {
                                                    obj2.time_response = front->wait_time;      // update P2 response time
                                                }
                                            RR2.MLFQ_add_to_queue(*this, obj2);
                                        }
                                    if (front->name == "P3")
                                        {
                                            cout << "front->name == P3" << endl;
                                            if (obj3.time_response == 0)
                                                {
                                                    obj3.time_response = front->wait_time;      // update P3 response time
                                                }
                                            RR2.MLFQ_add_to_queue(*this, obj3);
                                        }
                                    if (front->name == "P4")
                                        {
                                            cout << "front->name == P4" << endl;
                                            if (obj4.time_response == 0)
                                                {
                                                    obj4.time_response = front->wait_time;      // update P4 response time
                                                }
                                            RR2.MLFQ_add_to_queue(*this, obj4);
                                        }
                                    if (front->name == "P5")
                                        {
                                            cout << "front->name == P5" << endl;
                                            if (obj5.time_response == 0)
                                                {
                                                    obj5.time_response = front->wait_time;      // update P5 response time
                                                }
                                            RR2.MLFQ_add_to_queue(*this, obj5);
                                        }
                                    if (front->name == "P6")
                                        {
                                            cout << "front->name == P6" << endl;
                                            if (obj6.time_response == 0)
                                                {
                                                    obj6.time_response = front->wait_time;      // update P6 response time
                                                }
                                            RR2.MLFQ_add_to_queue(*this, obj6);
                                        }
                                    if (front->name == "P7")
                                        {
                                            cout << "front->name == P7" << endl;
                                            if (obj7.time_response == 0)
                                                {
                                                    obj7.time_response = front->wait_time;      // update P7 response time
                                                }
                                            RR2.MLFQ_add_to_queue(*this, obj7);
                                        }
                                    if (front->name == "P8")
                                        {
                                            cout << "front->name == P8" << endl;
                                            if (obj8.time_response == 0)
                                                {
                                                    obj8.time_response = front->wait_time;      // update P8 response time
                                                }
                                            RR2.MLFQ_add_to_queue(*this, obj8);
                                        }
                                    this->deQueue();                                            // remove node from READY_1 after it goes to READY_2
                                } // END else if (front->time_q > 0)
                        }   // END else if this->RR_time == 0)
                }   // ENDif (front->burst_type == 'C' || front->burst_type == 'c')
            else // if (front->burst_type == 'I' || front->burst_type == 'i')
                {
                    cout << "Invalid Burst_type for this queue" << endl;                        // debug: SHOULD NOT HAPPEN
                }
        }   // END if (!Empty())
    else if (!(RR2.Empty()))                    // if the 2nd ready queue is not empty: start processes in 2nd queue
        {
            if (RR2.front->burst_type == 'C' || RR2.front->burst_type == 'c')
                {
                    if (RR2.RR_time > 0)
                        {
                            if (RR2.front->time_q > 0)
                                {
                                    RR2.RR_time--;                                          // decrement the Round Robin Time
                                    RR2.front->time_q--;                                    // decrement the CPU burst time of process
                                    CPU_total_time++;                                       // Increment the CPU total time
                                    Global_clock++;                                         // Increment Global Clock
                                    RR2.Global_clock++;
                                    FC.Global_clock++;
                                    IO.MLFQ_IO_queue_timer(*this, obj1, obj2, obj3,
                                        obj4, obj5, obj6, obj7, obj8);                      // I/O time must be synced with global clock time.
                                    MLFQ_increment_wait(*this, RR2, FC);                    // increment all wait times
                                }
                            else // if (RR2.front->time_q == 0)                             // send the process to I/O anyway (RR_time expired)
                                {
                                    cout << "*** ADD TO IO!! ***" << endl;
                                    cout << "*** RESET RR timer ***" << endl;
                                    RR2.RR_time = 10;

                                    if (RR2.front->name == "P1")
                                        {
                                            cout << "RR2.front->name == P1" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj1);
                                        }
                                    if (RR2.front->name == "P2")
                                        {
                                            cout << "RR2.front->name == P2" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj2);
                                        }
                                    if (RR2.front->name == "P3")
                                        {
                                            cout << "RR2.front->name == P3" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj3);
                                        }
                                    if (RR2.front->name == "P4")
                                        {
                                            cout << "RR2.front->name == P4" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj4);
                                        }
                                    if (RR2.front->name == "P5")
                                        {
                                            cout << "RR2.front->name == P5" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj5);
                                        }
                                    if (RR2.front->name == "P6")
                                        {
                                            cout << "RR2.front->name == P6" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj6);
                                        }
                                    if (RR2.front->name == "P7")
                                        {
                                            cout << "RR2.front->name == P7" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj7);
                                        }
                                    if (RR2.front->name == "P8")
                                        {
                                            cout << "RR2.front->name == P8" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj8);
                                        }
                                    RR2.deQueue();                                                  // remove node from READY_2 queue
                                }   // END else // if (front->time_q == 0)
                        }   // END if (RR2.RR_time > 0)
                    else // if (RR2.RR_time == 0)
                        {
                            if (RR2.front->time_q == 0)
                                {
                                    cout << "*** ADD TO IO!! ***" << endl;
                                    cout << "*** RESET RR timer ***" << endl;
                                    RR2.RR_time = 10;

                                    if (RR2.front->name == "P1")
                                        {
                                            cout << "RR2.front->name == P1" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj1);
                                        }
                                    if (RR2.front->name == "P2")
                                        {
                                            cout << "RR2.front->name == P2" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj2);
                                        }
                                    if (RR2.front->name == "P3")
                                        {
                                            cout << "RR2.front->name == P3" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj3);
                                        }
                                    if (RR2.front->name == "P4")
                                        {
                                            cout << "RR2.front->name == P4" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj4);
                                        }
                                    if (RR2.front->name == "P5")
                                        {
                                            cout << "RR2.front->name == P5" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj5);
                                        }
                                    if (RR2.front->name == "P6")
                                        {
                                            cout << "RR2.front->name == P6" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj6);
                                        }
                                    if (RR2.front->name == "P7")
                                        {
                                            cout << "RR2.front->name == P7" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj7);
                                        }
                                    if (RR2.front->name == "P8")
                                        {
                                            cout << "RR2.front->name == P8" << endl;
                                            IO.MLFQ_add_to_queue(RR2, obj8);
                                        }
                                    RR2.deQueue();                                                  // remove node from READY_2 queue
                                }
                            else // if (RR2.front->time_q > 0)
                                {
                                    cout << "*** ADD TO Ready_3!! ***" << endl;
                                    cout << "*** Reset RR Timer ***" << endl;
                                    RR2.RR_time = 10;

                                    if (RR2.front->name == "P1")
                                        {
                                            cout << "RR2.front->name == P1" << endl;
                                            FC.MLFQ_add_to_queue(RR2, obj1);
                                        }
                                    if (RR2.front->name == "P2")
                                        {
                                            cout << "RR2.front->name == P2" << endl;
                                            FC.MLFQ_add_to_queue(RR2, obj2);
                                        }
                                    if (RR2.front->name == "P3")
                                        {
                                            cout << "RR2.front->name == P3" << endl;
                                            FC.MLFQ_add_to_queue(RR2, obj3);
                                        }
                                    if (RR2.front->name == "P4")
                                        {
                                            cout << "RR2.front->name == P4" << endl;
                                            FC.MLFQ_add_to_queue(RR2, obj4);
                                        }
                                    if (RR2.front->name == "P5")
                                        {
                                            cout << "RR2.front->name == P5" << endl;
                                            FC.MLFQ_add_to_queue(RR2, obj5);
                                        }
                                    if (RR2.front->name == "P6")
                                        {
                                            cout << "RR2.front->name == P6" << endl;
                                            FC.MLFQ_add_to_queue(RR2, obj6);
                                        }
                                    if (RR2.front->name == "P7")
                                        {
                                            cout << "RR2.front->name == P7" << endl;
                                            FC.MLFQ_add_to_queue(RR2, obj7);
                                        }
                                    if (RR2.front->name == "P8")
                                        {
                                            cout << "RR2.front->name == P8" << endl;
                                            FC.MLFQ_add_to_queue(RR2, obj8);
                                        }
                                    RR2.deQueue();                                              // remove node from READY_2 after it goes to READY_2
                                }   // END else if (RR2.front->time_q > 0)
                        }   // END else if (RR2.RR_time == 0)
                }   // END if (RR2.front->burst_type == 'C' || RR2.front->burst_type == 'c')
            else
                {
                    cout << "Invalid Burst_type for this queue" << endl;                        // debug: SHOULD NOT HAPPEN
                }
        }   // END else if (!(RR2.Empty()))
    else    // if 1st & 2nd Ready queues are Empty: Run the processes in the 3rd Ready queue
        {
            if (FC.Empty())             // if all 3 queues are empty
                {
                    cout << "All Ready Queues Empty: Increment CPU_idle_time" << endl;          // debug
                    Global_clock++;
                    RR2.Global_clock++;
                    FC.Global_clock++;
                    this->CPU_idle_time++;                                          // increment CPU idle times
                    RR2.CPU_idle_time++;
                    FC.CPU_idle_time++;
                    IO.MLFQ_IO_queue_timer(*this, obj1, obj2, obj3,
                        obj4, obj5, obj6, obj7, obj8);                              // I/O timer must be synced with Global clock.
                }
            else    // there is something in the 3rd queue
                {
                    if (FC.front->burst_type == 'C' || FC.front->burst_type == 'c')
                        {
                            if (FC.front->time_q > 0)                               // if there is still CPU time left, decrement the counter
                                {
                                    FC.front->time_q--;
                                    CPU_total_time++;                               // increment CPU busy time
                                    this->Global_clock++;                           // increment the global clock
                                    RR2.Global_clock++;
                                    FC.Global_clock++;
                                    IO.MLFQ_IO_queue_timer(*this, obj1, obj2, obj3,
                                        obj4, obj5, obj6, obj7, obj8);              // I/O time must be synced with global clock time.
                                    MLFQ_increment_wait(*this, RR2, FC);            // increment all wait times
                                }
                            else if (FC.front->time_q == 0)                         // if there is no time left, then CPU burst is done. Delete node from the queue.
                                {
                                    cout << "*** ADD TO IO!! ***" << endl;
                                    if (FC.front->name == "P1")
                                        {
                                            cout << "front->name == P1" << endl;
                                            IO.MLFQ_add_to_queue(FC, obj1);
                                        }
                                    if (FC.front->name == "P2")
                                        {
                                            cout << "front->name == P2" << endl;
                                            IO.MLFQ_add_to_queue(FC, obj2);
                                        }
                                    if (FC.front->name == "P3")
                                        {
                                            cout << "front->name == P3" << endl;
                                            IO.MLFQ_add_to_queue(FC, obj3);
                                        }
                                    if (FC.front->name == "P4")
                                        {
                                            cout << "front->name == P4" << endl;
                                            IO.MLFQ_add_to_queue(FC, obj4);
                                        }
                                    if (FC.front->name == "P5")
                                        {
                                            cout << "front->name == P5" << endl;
                                            IO.MLFQ_add_to_queue(FC, obj5);
                                        }
                                    if (FC.front->name == "P6")
                                        {
                                            cout << "front->name == P6" << endl;
                                            if (obj6.time_response == 0)
                                            IO.MLFQ_add_to_queue(FC, obj6);
                                        }
                                    if (FC.front->name == "P7")
                                        {
                                            cout << "front->name == P7" << endl;
                                            IO.MLFQ_add_to_queue(FC, obj7);
                                        }
                                    if (FC.front->name == "P8")
                                        {
                                            cout << "front->name == P8" << endl;
                                            IO.MLFQ_add_to_queue(FC, obj8);
                                        }
                                    FC.deQueue();
                                }
                            else
                                {
                                    cout << "CPU_queue_timer: Invalid Condition" << endl;               // debug statement
                                }
                        }   // END if (FC.front->burst_type == 'C' || FC.front->burst_type == 'c')
                    else
                        {
                            cout << "Invalid Burst_type for this queue" << endl;                        // debug: SHOULD NOT HAPPEN
                        }
                }   // END else if (!(FC.Empty())
        } // END else if 1st & 2nd are Empty: Run the processes in the 3rd Ready queue
}

void queue::MLFQ_IO_queue_timer(queue& CPU, Process& obj1, Process& obj2,               // calling object is the IO_wait queue
    Process& obj3, Process& obj4, Process& obj5, Process& obj6, Process& obj7, Process& obj8)
{
    cout << "Calling IO_Queue_Timer" << endl;

    node *p = this->front;                                                              // declare a pointer at the front of the IO_wait queue

    if (p != 0)                                                                         // if the pointer is NULL, then the queue is empty
        {
            if (p->burst_type == 'C' || p->burst_type == 'c')
                {
                    cout << "ERROR: Only Decrement I/O bursts in I/O queue" << endl;    // debug: Should NEVER happen
                    return;
                }
            else // if(!Empty())
                {
                    while (p != 0)
                        {
                            if (p->time_q > 0)
                                {
                                    p->time_q--;                                        // decrement the time in I/O
                                    p = p->next;                                        // traverse the entire I/O list
                                }
                            else // if (p->time_q == 0)
                                {
                                    cout << "*** Add " << p->name << " to ready list ***" << endl;
                                    if (p->name == "P1")
                                        {
                                            cout << "p->name == P1" << endl;
                                            CPU.MLFQ_add_to_queue(*this, obj1);
                                            cout << "*** Remove node from Process P1 ***" << endl;
                                            obj1.deQueue();                             // need to remove the CPU burst from the Process now
                                        }
                                    if (p->name == "P2")
                                        {
                                            cout << "p->name == P2" << endl;
                                            CPU.MLFQ_add_to_queue(*this, obj2);
                                            cout << "*** Remove node from Process P2 ***" << endl;
                                            obj2.deQueue();                             // need to remove the CPU burst from the Process now
                                        }
                                    if (p->name == "P3")
                                        {
                                            cout << "p->name == P3" << endl;
                                            CPU.MLFQ_add_to_queue(*this, obj3);
                                            cout << "*** Remove node from Process P3 ***" << endl;
                                            obj3.deQueue();                             // need to remove the CPU burst from the Process now
                                        }
                                    if (p->name == "P4")
                                        {
                                            cout << "p->name == P4" << endl;
                                            CPU.MLFQ_add_to_queue(*this, obj4);
                                            cout << "*** Remove node from Process P4 ***" << endl;
                                            obj4.deQueue();                             // need to remove the CPU burst from the Process now
                                        }
                                    if (p->name == "P5")
                                        {
                                            cout << "p->name == P5" << endl;
                                            CPU.MLFQ_add_to_queue(*this, obj5);
                                            cout << "*** Remove node from Process P5 ***" << endl;
                                            obj5.deQueue();                             // need to remove the CPU burst from the Process now
                                        }
                                    if (p->name == "P6")
                                        {
                                            cout << "p->name == P6" << endl;
                                            CPU.MLFQ_add_to_queue(*this, obj6);
                                            cout << "*** Remove node from Process P6 ***" << endl;
                                            obj6.deQueue();                             // need to remove the CPU burst from the Process now
                                        }
                                    if (p->name == "P7")
                                        {
                                            cout << "p->name == P7" << endl;
                                            CPU.MLFQ_add_to_queue(*this, obj7);
                                            cout << "*** Remove node from Process P7 ***" << endl;
                                            obj7.deQueue();                             // need to remove the CPU burst from the Process now
                                        }
                                    if (p->name == "P8")
                                        {
                                            cout << "p->name == P8" << endl;
                                            CPU.MLFQ_add_to_queue(*this, obj8);
                                            cout << "*** Remove node from Process P8 ***" << endl;
                                            obj8.deQueue();                             // need to remove the CPU burst from the Process now
                                        }
                                    cout << "*** Remove from I/O ***" << endl;
                                    remove(p);                                          // delete the node from I/O queue if its finished I/O
                                    p = p->next;                                        // continue traversing the list
                                }
                        }
                    IO_total_time++;
                }
        }
    else
        {
            cout << "IO_Queue empty: Don't adjust IO_total_time" << endl;
        }
}

void queue::Print_ready_IO(string name_of_proc)
{
    cout << "Print_ready_IO Called" << endl;

    if (this->queue_name == "Ready")                                                    // For FCFS, SJF queues only
        {
            cout << "Printing process data for: {" << name_of_proc << "}" << endl;
            cout << "queue_name = " << queue_name << endl;
            cout << "# nodes remaining: " << nodes_remaining << endl;
            cout << "CPU idle time: " << CPU_idle_time << endl;
            cout << "CPU time count: " << CPU_total_time << endl;
            cout << "Global_clock: " << Global_clock << endl;
        }
    else if (this->queue_name == "Ready_1" || this->queue_name == "Ready_2")            // For 1st & 2nd RR queues only: tracks global time
        {
            cout << "Printing process data for: {" << name_of_proc << "}" << endl;
            cout << "queue_name = " << queue_name << endl;
            cout << "# nodes remaining: " << nodes_remaining << endl;
            cout << "CPU idle time: " << CPU_idle_time << endl;
            cout << "CPU time count: " << CPU_total_time << endl;
            cout << "RR_time: " << RR_time << endl;
            cout << "Global_clock: " << Global_clock << endl;
        }
    else if (this->queue_name == "Ready_3")                                             // For 3rd FCFS queue only: NO RR time
        {
            cout << "Printing process data for: {" << name_of_proc << "}" << endl;
            cout << "queue_name = " << queue_name << endl;
            cout << "# nodes remaining: " << nodes_remaining << endl;
            cout << "CPU idle time: " << CPU_idle_time << endl;
            cout << "CPU time count: " << CPU_total_time << endl;
            cout << "Global_clock: " << Global_clock << endl;
        }
    else if (this->queue_name == "IO_wait")                                             // for the I/O queue ONLY
        {
            cout << "Printing process data for: {" << name_of_proc << "}" << endl;
            cout << "queue_name = " << queue_name << endl;
            cout << "# nodes remaining: " << nodes_remaining << endl;
            cout << "IO time count: " << IO_total_time << endl;
        }

    node *p = this->front;                                                              // declare a pointer to point to the front

	if (p == 0)
        {
            cout << "Zero Nodes in Queue...Nothing to print" << endl;
        }
	else // if (p != 0)
        {
            while(p != 0)                                                               // print the data in each node
                {
                    cout << "=> Node #: " << p->node_number << " Name: " << p->name << " Time_q: "
                         << p->time_q << " Type: " << p->burst_type << " Wait time: " << p->wait_time << " <=" << endl;
                    p = p->next;                                                        // traverse to the next node
                }
        }
    cout << endl;
}

void queue::Results(Process& obj1, Process& obj2, Process& obj3,                        // calling object is the READY queue
    Process& obj4, Process& obj5, Process& obj6, Process& obj7, Process& obj8)
{

    double CPU_utilization;
    double avg_wait_time;
    double avg_turnaround_time;
    double avg_response_time;

    cout.setf(ios::fixed);
    cout.precision(2);

    CPU_utilization = (this->CPU_total_time * 100.0) / this->Global_clock;

    avg_wait_time = (obj1.total_wait_time + obj2.total_wait_time + obj3.total_wait_time
        + obj4.total_wait_time + obj5.total_wait_time + obj6.total_wait_time
        + obj7.total_wait_time + obj8.total_wait_time) / 8.0;

    avg_turnaround_time = (obj1.time_finished + obj2.time_finished + obj3.time_finished
        + obj4.time_finished + obj5.time_finished + obj6.time_finished
        + obj7.time_finished + obj8.time_finished) / 8.0;

    avg_response_time = (obj1.time_response + obj2.time_response + obj3.time_response
        + obj4.time_response + obj5.time_response + obj6.time_response
        + obj7.time_response + obj8.time_response) / 8.0;

    cout << " Process    " << "|    T_w     |" << "    T_tr    |" << "     T_r    |" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << " " << left << setw(10) << obj1.proc_name << " | " << left << setw(10) << obj1.total_wait_time
         << " | " << left << setw(10) << obj1.time_finished << " | " << left << setw(10) << obj1.time_response << " |" << endl;
    cout << " " << left << setw(10) << obj2.proc_name << " | " << left << setw(10) << obj2.total_wait_time
         << " | " << left << setw(10) << obj2.time_finished << " | " << left << setw(10) << obj2.time_response << " |" << endl;
    cout << " " << left << setw(10) << obj3.proc_name << " | " << left << setw(10) << obj3.total_wait_time
         << " | " << left << setw(10) << obj3.time_finished << " | " << left << setw(10) << obj3.time_response << " |" << endl;
    cout << " " << left << setw(10) << obj4.proc_name << " | " << left << setw(10) << obj4.total_wait_time
         << " | " << left << setw(10) << obj4.time_finished << " | " << left << setw(10) << obj4.time_response << " |" << endl;
    cout << " " << left << setw(10) << obj5.proc_name << " | " << left << setw(10) << obj5.total_wait_time
         << " | " << left << setw(10) << obj5.time_finished << " | " << left << setw(10) << obj5.time_response << " |" << endl;
    cout << " " << left << setw(10) << obj6.proc_name << " | " << left << setw(10) << obj6.total_wait_time
         << " | " << left << setw(10) << obj6.time_finished << " | " << left << setw(10) << obj6.time_response << " |" << endl;
    cout << " " << left << setw(10) << obj7.proc_name << " | " << left << setw(10) << obj7.total_wait_time
         << " | " << left << setw(10) << obj7.time_finished << " | " << left << setw(10) << obj7.time_response << " |" << endl;
    cout << " " << left << setw(10) << obj8.proc_name << " | " << left << setw(10) << obj8.total_wait_time
         << " | " << left << setw(10) << obj8.time_finished << " | " << left << setw(10) << obj8.time_response << " |" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << " " << left << "Average:   | " << left << setw(10) << avg_wait_time
         << " | " << left << setw(10) << avg_turnaround_time << " | " << left << setw(10) << avg_response_time << " |" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << "CPU Utilization (CPU_u) = " << CPU_utilization << '%' << endl;
    cout << "Total Time to complete all Processes = " << Global_clock << endl;

}
