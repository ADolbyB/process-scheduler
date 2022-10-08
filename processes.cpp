/**
Joel Brigida
COP4610-002
Dr. Borijovie Furht
Programming Term Project: Process Scheduler
Description:
    This header file is the implementation file for Processes Queue Class
*/

#include <iostream>
#include <string>
#include <fstream>

#include "processes.h"

using namespace std;

Process::Process(string the_name)                           // initialize an empty process
{

    //cout << "Default Constructor Called" << endl;

    front = back = 0;                                       // set front and back pointers to NULL
    proc_name = the_name;                                   // initialize process name to a NULL string
    total_wait_time = 0;                                    // initialize total wait time of process to zero
    time_finished = 0;
    time_response = 0;
    nodes_remaining = 0;                                    // set nodes_remaining to 0
    total_nodes = 0;                                        // set node_count to 0

}

Process::~Process()
{
	//cout << "* Destructor Called *" << endl;

	while (!Empty())
        {
            deQueue();                                      // delete nodes until the list is empty
        }
}

void Process::Make_process(string filename, string obj_name)
{

    cout << "Constructing Process from " << filename << endl;

    ifstream file_in;                                       // object to read the integers from file
    int next_value;                                         // takes value of number read from file

    file_in.open(filename);
    if(file_in.fail())
        {
            cout << "Function \'Make_Process\' failed to open input file...exiting program" << endl;
            exit(1);
        }

    while (file_in >> next_value)
        {
            add_back(next_value, obj_name);                 // add a node w/ burst time and the process name which it originated.
        }

    file_in.close();

}

void Process::add_back(const int& time, const string& any_name) // adds a new node to the BACK
{
    //cout << "Adding a node to the back..." << endl;

	if (Empty())
        {
            front = back = new node;                        // declare a new node, set front and back to point to the same new node.
            nodes_remaining++;                              // increment the # of nodes remaining in the queue
            total_nodes++;                                  // increment total number of nodes in the queue
            front->wait_time = 0;                           // set initial wait time to zero
            front->time_q = time;                           // add the time value to the new node
            front->node_number = 1;                         // 1st node is always = 1
            front->burst_type = 'C';                        // 1st node is always a CPU burst
            front->name = any_name;                         // need to know which process this node originated from
            front->next = 0;                                // since it's a single node queue, need to set "next" and "prev" fields to NULL
        }
	else // if not empty
        {
            back->next = new node;                          // declare a new node after the current back node. (default constructor makes *back & *front of new node = NULL already
            nodes_remaining++;                              // increment the # of nodes remaining in the queue
            total_nodes++;                                  // increment total number of nodes in the queue
            back->wait_time = 0;
            back = back->next;                              // move the back pointer to the new node, which is now in the back
            back->name = any_name;                          // need to know which process this node originated from
            back->time_q = time;                            // add the data to the new node
            back->node_number = total_nodes;                // new node takes position in the queue as the total # of nodes.

            if (total_nodes % 2 == 0)
                {
                    back->burst_type = 'I';                 // Even nodes are CPU bursts
                }

            else // if (total_nodes % 2 != 0)
                {
                    back->burst_type = 'C';                 // Odd nodes are I/O bursts
                }

            back->next = 0;                                 // set the "next" field of the back node to NULL
        }
}

void Process::deQueue()                                     // deletes a node from the FRONT
{
    //cout << "Deleting front node..." << endl;

	if (!Empty())
        {
            node *p = front;                                // declare pointer to point to the front node.

            front = front->next;                            // move the front pointer to the next one down the queue

            if (front == 0)                                 // if front is NULL after moving pointer, then there are no nodes left in the queue,
                {
                    back = 0;                               // so set the back pointer to NULL also (as a precaution)
                }

            delete p;                                       // delete unwanted node.
            nodes_remaining--;                              // decrement the # of nodes remaining in queue
        }
	else
        {
            cout << "Cannot deQueue because queue is empty" << endl;
        }
}

void Process::Print_proc_nodes(string name_of_proc)
{
    cout << "Print_proc_nodes Called" << endl;

	node *p = this->front;                                  // declare a pointer to point to the front

	cout << "Printing process data for: {" << name_of_proc << "}" << endl;
	cout << "proc_name = " << proc_name << endl;
	cout << "# nodes remaining: " << nodes_remaining << endl;
	cout << "# total nodes created: " << total_nodes << endl;
	cout << "total_wait_time: " << total_wait_time << endl;
	cout << "Process time_finished: " << time_finished << endl;
	cout << "Process time_response: " << time_response << endl;

	if (p == 0)
        {
            cout << "Zero Nodes in Queue...Nothing to print" << endl;
        }
	else // if (p != 0)
        {
            while(p != 0)                                   // print the data in each node
                {
                    cout << "(Node #: " << p->node_number << " Name: " << p->name << " Time_q: "
                         << p->time_q << " Type: " << p->burst_type << " Wait_time " << p->wait_time << ")" << endl;
                    p = p->next;                            // traverse to the next node
                }
        }
    cout << endl;
}
