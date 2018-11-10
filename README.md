This program is a Discrete Event Simulator, based on Giogrio's Discrete Event Simulator.
It is a simple model of how processes move through a computing system.

A basic, high level overview of how this simulation works is as follows:
When the simulation starts, a job arrived at CPU event is generated and added to the event queue.
Once a job arrives at the CPU, it spends some time being processed. Then, a random number is generated
and based on that, the program decides whether the job will exit the simulation or go to DISK for
I/O processing. If the program decides to send the job to DISK, it checks which of DISK 1 or DISK 2
has a shorter queue of jobs. It sends the job to the DISK with the shorter queue. If both queues
are of equal length, it generates a random number to decide which DISK to send the job to.
When the job arrives at DISK, it spends some time being processed there. Once the job finishes I/O
processing at DISK, the program generates a random number to determine whether the job will exit the
simulation to go back to the CPU. If the program decides to send the job back to the CPU, it adds
the job to the CPU queue. The simulation continues in this loop until a "Simulation Ended Event" is
pulled off the event queue. At the start of every iteration of the loop, a new job arrived event is
created.

This is how the program is set up. There are 3 files that contain the code for this simulation. In
one file, called “fqueue.c”, I have only written code that implements a FIFO queue as a linked
list data structure. This file contains a structure called “fNode” which is the node I have created
to be used as the node for all FIFO queues. In this file, there are also functions for the FIFO
queue, such as “fEnqueue” and “fDequeue”, which are respectively used to insert and remove
elements from the queue.

In the second file, called “pqueue.c”, I have only written code that implements a Priority queue
as a linked list data structure. This file contains a structure called “pNode” which is the node I
have created to be used as the node for any Priority queues in the program. In this file, there
are also functions for the Priority queue, such as “pEnqueue” and “pDequeue”, which are
respectively used to insert and remove elements from the queue. The insertion function is what
prioritizes the list and makes the queue a Priority queue.

Elements are prioritised based on their “priorityNum” field. This field is used to represent the
time of that specific event/element. In my code, anywhere I have used the term “priorityNum” it
is referring to and dealing with the time of that event/job/element.