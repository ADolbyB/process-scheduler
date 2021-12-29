/**
Joel Brigida
COP4610-002
Dr. Boijovie Furht
Programming Term Project: Process Scheduler
Description:
    This is the driver file for the CPU Process Scheduler.
*/

#include <iostream>
#include <string>
#include <fstream>

#include "processes.h"

using namespace std;

void Menu(void);
void FCFS(void);
void SJF(void);
void MLFQ(void);

int main()
{

    Menu();

    return 0;
}

void Menu(void)
{

    char choice;

    do
    {
        cout << "=>> Process Scheduler <<=" << endl << endl;
        cout << "*** Make a Selection: ***" << endl;
        cout << "1) Run FCFS Algorithm" << endl;
        cout << "2) Run SJF Algorithm" << endl;
        cout << "3) Run MLFQ Algorithm" << endl;
        cout << "4) Exit" << endl << endl;
        cin >> choice;

        switch(choice)
            {
                case '1':
                    cout << endl << "*** Running FCFS Algorithm ***" << endl << endl;
                    FCFS();
                    cout << endl << "*** End of FCFS Algorithm ***" << endl << endl;
                    cout << endl;
                    break;

                case '2':
                    cout << endl << "*** Running SJF Algorithm ***" << endl << endl;
                    SJF();
                    cout << endl << "*** End of SJF Algorithm ***" << endl << endl;
                    cout << endl;
                    break;

                case '3':
                    cout << endl << "*** Running MLFQ Algorithm ***" << endl << endl;
                    MLFQ();
                    cout << endl << "*** End of MLFQ Algorithm ***" << endl << endl;
                    cout << endl;
                    break;

                default:
                    cout << endl << "Goodbye!!" << endl << endl;
            }
    } while(choice == '1' || choice == '2' || choice == '3');

}

void FCFS(void) // First Come First Served Algorithm
{

    Process P1("P1"), P2("P2"), P3("P3"), P4("P4"),
        P5("P5"), P6("P6"), P7("P7"), P8("P8");                 // Declare 8 processes as linked lists

    queue Ready("Ready", 'F'), IO_wait("IO_wait", 'I');         // Declare 2 queues as ready and IO queues

    P1.Make_process("p1.txt", "P1");                            // Build each process into a linked list
    P2.Make_process("p2.txt", "P2");
    P3.Make_process("p3.txt", "P3");
    P4.Make_process("p4.txt", "P4");
    P5.Make_process("p5.txt", "P5");
    P6.Make_process("p6.txt", "P6");
    P7.Make_process("p7.txt", "P7");
    P8.Make_process("p8.txt", "P8");

    cout << endl;

    P1.Print_proc_nodes("P1");                                  // debug: check if Processes were built properly
    P2.Print_proc_nodes("P2");
    P3.Print_proc_nodes("P3");
    P4.Print_proc_nodes("P4");
    P5.Print_proc_nodes("P5");
    P6.Print_proc_nodes("P6");
    P7.Print_proc_nodes("P7");
    P8.Print_proc_nodes("P8");

    Ready.Print_ready_IO("Ready_Queue");                        // debug: Ready list should be EMPTY now

    IO_wait.Print_ready_IO("IO_Queue");                         // debug: IO_Wait list should be EMPTY now.

    Ready.FCFS_SJF_add_to_queue(IO_wait, P1);                   // load initial processes into ready queue at TIME = 0
    Ready.FCFS_SJF_add_to_queue(IO_wait, P2);
    Ready.FCFS_SJF_add_to_queue(IO_wait, P3);
    Ready.FCFS_SJF_add_to_queue(IO_wait, P4);
    Ready.FCFS_SJF_add_to_queue(IO_wait, P5);
    Ready.FCFS_SJF_add_to_queue(IO_wait, P6);
    Ready.FCFS_SJF_add_to_queue(IO_wait, P7);
    Ready.FCFS_SJF_add_to_queue(IO_wait, P8);

    cout << endl;

    Ready.Print_ready_IO("Ready_Queue");                        // debug: should be 8 processes initially loaded into the Ready Queue

    P1.Print_proc_nodes("P1");                                  // debug: Each node should be missing the 1st CPU burst node
    P2.Print_proc_nodes("P2");
    P3.Print_proc_nodes("P3");
    P4.Print_proc_nodes("P4");
    P5.Print_proc_nodes("P5");
    P6.Print_proc_nodes("P6");
    P7.Print_proc_nodes("P7");
    P8.Print_proc_nodes("P8");

    cout << endl;

    while (!(Ready.Empty() && IO_wait.Empty()))
        {
            Ready.FCFS_SJF_CPU_queue_timer(IO_wait, P1, P2, P3, P4, P5, P6, P7, P8);
            Ready.Print_ready_IO("Ready_For_loop");
            IO_wait.Print_ready_IO("IO_For_Loop");
        }


    P1.Print_proc_nodes("P1_post_scheduler");
    P2.Print_proc_nodes("P2_post_scheduler");
    P3.Print_proc_nodes("P3_post_scheduler");
    P4.Print_proc_nodes("P4_post_scheduler");
    P5.Print_proc_nodes("P5_post_scheduler");
    P6.Print_proc_nodes("P6_post_scheduler");
    P7.Print_proc_nodes("P7_post_scheduler");
    P8.Print_proc_nodes("P8_post_scheduler");

    Ready.Print_ready_IO("Ready_Queue_post_scheduler");

    IO_wait.Print_ready_IO("IO_Queue_post_scheduler");

    Ready.Results(P1, P2, P3, P4, P5, P6, P7, P8);

}

void SJF(void)  // Shortest Job 1st Algorithm
{

    Process P1("P1"), P2("P2"), P3("P3"), P4("P4"),
        P5("P5"), P6("P6"), P7("P7"), P8("P8");                 // Declare 8 processes as linked lists

    queue Ready("Ready", 'S'), IO_wait("IO_wait", 'I');         // Declare 2 queues as ready and IO queues

    P1.Make_process("p1.txt", "P1");                            // Build each process into a linked list
    P2.Make_process("p2.txt", "P2");
    P3.Make_process("p3.txt", "P3");
    P4.Make_process("p4.txt", "P4");
    P5.Make_process("p5.txt", "P5");
    P6.Make_process("p6.txt", "P6");
    P7.Make_process("p7.txt", "P7");
    P8.Make_process("p8.txt", "P8");

    cout << endl;

    P1.Print_proc_nodes("P1");                                  // debug: check if Processes were built properly
    P2.Print_proc_nodes("P2");
    P3.Print_proc_nodes("P3");
    P4.Print_proc_nodes("P4");
    P5.Print_proc_nodes("P5");
    P6.Print_proc_nodes("P6");
    P7.Print_proc_nodes("P7");
    P8.Print_proc_nodes("P8");

    Ready.Print_ready_IO("Ready_Queue");                        // debug: Ready list should be EMPTY now

    IO_wait.Print_ready_IO("IO_Queue");                         // debug: IO_Wait list should be EMPTY now.

    Ready.FCFS_SJF_add_to_queue(IO_wait, P4);                   // load initial processes into ready queue at TIME = 0
    Ready.FCFS_SJF_add_to_queue(IO_wait, P2);                   // initial load done manually in order of shortest CPU burst 1st
    Ready.FCFS_SJF_add_to_queue(IO_wait, P8);                   // P4 = 3, P2 = 4, P8 = 4, P1 = 5, P3 = 8, P6 = 11, P7 = 14, P5 = 16
    Ready.FCFS_SJF_add_to_queue(IO_wait, P1);
    Ready.FCFS_SJF_add_to_queue(IO_wait, P3);
    Ready.FCFS_SJF_add_to_queue(IO_wait, P6);
    Ready.FCFS_SJF_add_to_queue(IO_wait, P7);
    Ready.FCFS_SJF_add_to_queue(IO_wait, P5);

    cout << endl;

    Ready.Print_ready_IO("Ready_Queue");                        // debug: should be 8 processes initially loaded into the Ready Queue

    P1.Print_proc_nodes("P1");                                  // debug: Each node should be missing the 1st CPU burst node
    P2.Print_proc_nodes("P2");
    P3.Print_proc_nodes("P3");
    P4.Print_proc_nodes("P4");
    P5.Print_proc_nodes("P5");
    P6.Print_proc_nodes("P6");
    P7.Print_proc_nodes("P7");
    P8.Print_proc_nodes("P8");

    cout << endl;

        while (!(Ready.Empty() && IO_wait.Empty()))
        {
            Ready.FCFS_SJF_CPU_queue_timer(IO_wait, P1, P2, P3, P4, P5, P6, P7, P8);
            Ready.Print_ready_IO("Ready_For_loop");
            IO_wait.Print_ready_IO("IO_For_Loop");
        }


    P1.Print_proc_nodes("P1_post_scheduler");
    P2.Print_proc_nodes("P2_post_scheduler");
    P3.Print_proc_nodes("P3_post_scheduler");
    P4.Print_proc_nodes("P4_post_scheduler");
    P5.Print_proc_nodes("P5_post_scheduler");
    P6.Print_proc_nodes("P6_post_scheduler");
    P7.Print_proc_nodes("P7_post_scheduler");
    P8.Print_proc_nodes("P8_post_scheduler");

    Ready.Print_ready_IO("Ready_Queue_post_scheduler");

    IO_wait.Print_ready_IO("IO_Queue_post_scheduler");

    Ready.Results(P1, P2, P3, P4, P5, P6, P7, P8);

}

void MLFQ(void)
{
    Process P1("P1"), P2("P2"), P3("P3"), P4("P4"),
    P5("P5"), P6("P6"), P7("P7"), P8("P8");                         // Declare 8 processes as linked lists

    queue Ready_1("Ready_1", 'R', 5), Ready_2("Ready_2", 'R', 10),
        Ready_3("Ready_3", 'F'), IO_wait("IO_wait", 'I');           // Declare 4 queues: 3 ready and 1 I/O

    P1.Make_process("p1.txt", "P1");                                // Build each process into a linked list
    P2.Make_process("p2.txt", "P2");
    P3.Make_process("p3.txt", "P3");
    P4.Make_process("p4.txt", "P4");
    P5.Make_process("p5.txt", "P5");
    P6.Make_process("p6.txt", "P6");
    P7.Make_process("p7.txt", "P7");
    P8.Make_process("p8.txt", "P8");

    cout << endl;

    P1.Print_proc_nodes("P1");                                      // debug: check if Processes were built properly
    P2.Print_proc_nodes("P2");
    P3.Print_proc_nodes("P3");
    P4.Print_proc_nodes("P4");
    P5.Print_proc_nodes("P5");
    P6.Print_proc_nodes("P6");
    P7.Print_proc_nodes("P7");
    P8.Print_proc_nodes("P8");

    Ready_1.Print_ready_IO("Ready 1: RR-5");                        // debug: Ready list should be EMPTY now
    Ready_2.Print_ready_IO("Ready 2: RR-10");
    Ready_3.Print_ready_IO("Ready 3: FCFS");

    IO_wait.Print_ready_IO("IO_Queue");                             // debug: IO_Wait list should be EMPTY now.

    Ready_1.MLFQ_add_to_ready1(IO_wait, P1);                              // load initial processes into ready queue at TIME = 0
    Ready_1.MLFQ_add_to_ready1(IO_wait, P2);
    Ready_1.MLFQ_add_to_ready1(IO_wait, P3);
    Ready_1.MLFQ_add_to_ready1(IO_wait, P4);
    Ready_1.MLFQ_add_to_ready1(IO_wait, P5);
    Ready_1.MLFQ_add_to_ready1(IO_wait, P6);
    Ready_1.MLFQ_add_to_ready1(IO_wait, P7);
    Ready_1.MLFQ_add_to_ready1(IO_wait, P8);

    cout << endl;

    Ready_1.Print_ready_IO("Ready 1: RR-5");                    // debug: should be 8 processes initially loaded into the 1st Ready Queue
    Ready_2.Print_ready_IO("Ready 2: RR-10");
    Ready_3.Print_ready_IO("Ready 3: FCFS");

    P1.Print_proc_nodes("P1");                                  // debug: Each node should be missing the 1st CPU burst node
    P2.Print_proc_nodes("P2");
    P3.Print_proc_nodes("P3");
    P4.Print_proc_nodes("P4");
    P5.Print_proc_nodes("P5");
    P6.Print_proc_nodes("P6");
    P7.Print_proc_nodes("P7");
    P8.Print_proc_nodes("P8");

    cout << endl;

    while (!(Ready_1.Empty() && Ready_2.Empty() && Ready_3.Empty() && IO_wait.Empty()))
        {
            cout << endl << "*** TIME +1: NEW LOOP ITERATION***" << endl;
            Ready_1.MLFQ_CPU_timer(IO_wait, Ready_2, Ready_3, P1, P2, P3, P4, P5, P6, P7, P8);
            Ready_1.Print_ready_IO("Ready 1: RR-5");
            Ready_2.Print_ready_IO("Ready 2: RR-10");
            Ready_3.Print_ready_IO("Ready 3: FCFS");
            IO_wait.Print_ready_IO("IO_For_Loop");
//            P1.Print_proc_nodes("P1");
//            P2.Print_proc_nodes("P2");
//            P3.Print_proc_nodes("P3");
//            P4.Print_proc_nodes("P4");
//            P5.Print_proc_nodes("P5");
//            P6.Print_proc_nodes("P6");
//            P7.Print_proc_nodes("P7");
//            P8.Print_proc_nodes("P8");
        }

    P1.Print_proc_nodes("P1_post_scheduler");
    P2.Print_proc_nodes("P2_post_scheduler");
    P3.Print_proc_nodes("P3_post_scheduler");
    P4.Print_proc_nodes("P4_post_scheduler");
    P5.Print_proc_nodes("P5_post_scheduler");
    P6.Print_proc_nodes("P6_post_scheduler");
    P7.Print_proc_nodes("P7_post_scheduler");
    P8.Print_proc_nodes("P8_post_scheduler");

    Ready_1.Print_ready_IO("Ready_1_Queue_post_scheduler");
    Ready_2.Print_ready_IO("Ready_2_Queue_post_scheduler");
    Ready_3.Print_ready_IO("Ready_3_Queue_post_scheduler");

    IO_wait.Print_ready_IO("IO_Queue_post_scheduler");

    Ready_1.Results(P1, P2, P3, P4, P5, P6, P7, P8);

}
