#include "pqueue.c"
#include "fqueue.c"
#include <time.h>
#include <string.h>

/*-------------------------- Global pointer variables -----------------------*/
/*------------ Pointers to the CPU, Disk1, Disk2 and Event queues -------------*/

struct fNode* cpuFront; //Pointer to node for the front of the FIFO queue for the CPU
struct fNode* cpuRear;//Pointer to node for the rear of the FIFO queue for the CPU

struct fNode* disk1Front; //Pointer to node for the front of the FIFO queue for Disk 1
struct fNode* disk1Rear;//Pointer to node for the rear of the FIFO queue for the Disk 1

struct fNode* disk2Front; //Pointer to node for the front of the FIFO queue for Disk 2
struct fNode* disk2Rear;//Pointer to node for the rear of the FIFO queue for the Disk 2

struct pNode* eventsFront; //Pointer to node for the front of the priority queue for Events
struct pNode* eventsRear; //Pointer to node for the rear of the priority queue for Events

/*-Pointers to queues that keep track of how many jobs/events are in -CPU, Disk1, Disk2 and Events- queues for each iteration of the loop-*/
/*-during the course of the simulation-*/

struct fNode* cpuSizeQFront; //Pointer to node for the front of the FIFO queue that keeps track of CPU queue size
struct fNode* cpuSizeQRear; //Pointer to node for the rear of the FIFO queue that keeps track of CPU queue size

struct fNode* disk1SizeQFront; //Pointer to node for the front of the FIFO queue that keeps track of Disk1 queue size
struct fNode* disk1SizeQRear; //Pointer to node for the rear of the FIFO queue that keeps track of Disk1 queue size

struct fNode* disk2SizeQFront; //Pointer to node for the front of the FIFO queue that keeps track of Disk2 queue size
struct fNode* disk2SizeQRear; //Pointer to node for the rear of the FIFO queue that keeps track of Disk2 queue size

struct fNode* eventsSizeQFront; //Pointer to node for the front of the FIFO queue that keeps track of the Events queue size
struct fNode* eventsSizeQRear; //Pointer to node for the rear of the FIFO queue that keeps track of the Events queue size

/*------ Pointers to queues that keep track of how much time is taken to complete every job that enters CPU, Disk1 and Disk2 --------*/
/*-during the course of the simulation-*/
struct fNode* processTimeCpuFront; //Pointer to node for the front of the FIFO queue that keeps track of Job Completion Time at CPU
struct fNode* processTimeCpuRear; //Pointer to node for the rear of the FIFO queue that keeps track of Job Completion Time at CPU

struct fNode* processTimeDisk1Front; //Pointer to node for the front of the FIFO queue that keeps track of Job Completion Time at Disk1
struct fNode* processTimeDisk1Rear; //Pointer to node for the rear of the FIFO queue that keeps track of Job Completion Time at Disk1

struct fNode* processTimeDisk2Front; //Pointer to node for the front of the FIFO queue that keeps track of Job Completion Time at Disk2
struct fNode* processTimeDisk2Rear; //Pointer to node for the rear of the FIFO queue that keeps track of Job Completion Time at Disk2

//Function to generate random number
int randomNumber(int upper, int lower){
	return (rand() % (upper-lower+1)) +lower;
}




int main(){
//NOTE: The variable named "priorityNum" refers to the TIME of the Event. 

/*---------------------------- Variables --------------------------*/	
	//To store values affecting simulation
	int INIT_TIME, FIN_TIME, ARRIVE_MIN, ARRIVE_MAX, QUIT_PROB, CPU_MIN, CPU_MAX, DISK1_MIN, DISK1_MAX, DISK2_MIN, DISK2_MAX, SEED;
	
	//To keep track of job number
	int jobNum = 1;
	//To keep track of runtime
	int runtime = 0;
	//Boolean representative for simulation status
	int simFinished = 0;
	//To keep track of jobs completed at CPU
	double totalJobsFinCpu = 0;
	//To keep track of jobs completed at Disk1
	double totalJobsFinDisk1 = 0;
	//To keep track of jobs completed at Disk2
	double totalJobsFinDisk2 = 0;
	
	//Variables to store sizes of CPU, Disk1 and Disk2 queues
	double cpuQSize, disk1QSize, disk2QSize, eventsQSize;
	//Loop counter to keep track of how many times the loop ran over the course of the simulation
	//It is also equal to the total number of events
	double loopCount = 0;

	//Seed for random
	srand(time(0));

/*-------------- Opening, reading and writing to files --------------*/

   	//Create file pointer for writing to file
	FILE * fp;
	//Open file for writing
	fp = fopen ("log.txt","w");

	//Read file with values for the simulation variables
	FILE* file = fopen("values.txt", "r");
	char line[256];
	char *token;
	int vals[12];
	int i = 0;
	
	//Iterates over every line in the file
	while (fgets(line, sizeof(line), file)) {
		//Loads name of variable eg. CPU_MIN
		token = strtok(line, " ");
		fprintf(fp, "%s ", token);

		//Load time value of the variable eg. 20
		//Converts that string number into an integer value
		//Then stores that number in an array of integers that store the values read in from the config file
		token = strtok(NULL, ", ");
		fprintf(fp, "%s \n", token);
		vals[i] = atoi(token);
		i++;
	}
	//Close the file
	fclose(file);
	
	
	//Assign values to the variables from the array that holds the values read in from the config file
	INIT_TIME = vals[0];
	FIN_TIME = vals[1];
	ARRIVE_MIN = vals[2];
	ARRIVE_MAX = vals[3];
	QUIT_PROB = vals[4];
	CPU_MIN = vals[5];
	CPU_MAX = vals[6];
	DISK1_MIN = vals[7];
	DISK1_MAX = vals[8];
	DISK2_MIN = vals[9];
	DISK2_MAX = vals[10];
	SEED = vals[11];

	//Updating runtime with the value read in from file
	runtime = INIT_TIME;

/*---------------------Simulation Begins---------------------*/

	//Add simulation finished event to the events queue
	pEnqueue(&eventsFront, &eventsRear, jobNum, FIN_TIME, 4);
	//Add Job 1 arrived event to the event queue
	pEnqueue(&eventsFront, &eventsRear, jobNum, runtime, 0);	

	//While the queue is not empty and the simulation is not over
	while(!pIsEmpty(&eventsFront) && !simFinished){
		//Incrementing loop counter everytime the loop runs
		loopCount++;
		//At the start of each iteration of the loop, we create a new job arrives event
		//Increment job number
		jobNum++;
		//Calculating time taken for job to arrive at CPU
		int timeToArrive = randomNumber(ARRIVE_MAX, ARRIVE_MIN);
		runtime+= timeToArrive;
		//Adding job arrives event to the events queue
		pEnqueue(&eventsFront, &eventsRear, jobNum, runtime, 0);
		

		//See which event is next in line in events queue
		struct pNode* temp = (struct pNode*)malloc(sizeof(struct pNode*));
		//Store the event in temp
		temp = pPeek(&eventsFront);
		
		//If event is of type "Job Arrived" 
		if(temp->eventType == 0){
			//We add the job to the CPU queue
			fEnqueue(&cpuFront, &cpuRear, temp->data, temp->priorityNum);
			printf("\nAt time %d Job %d arrives.", temp->priorityNum, temp->data);
			fprintf(fp, "\nAt time %d Job %d arrives.", temp->priorityNum, temp->data);

			//As the job is sent to the CPU queue, an event is added to the event queue that is of type job finished at CPU			
			//Calculating time taken for job to be processed at CPU
			int timeTaken = randomNumber(CPU_MAX, CPU_MIN);	
			//Adding time taken for processing to queue holding process times for all jobs that enter the CPU
			fEnqueue(&processTimeCpuFront, &processTimeCpuRear, timeTaken, 0);
			runtime+= timeTaken;
			//Adding job finished at CPU event to the events queue
			pEnqueue(&eventsFront, &eventsRear,temp->data, runtime, 1);
		} 
		//Else if event type is of "Job Finished at CPU"
		else if(temp->eventType == 1){
			//Increment number of jobs completed/finished at CPU
			totalJobsFinCpu++;

			printf("\nAt time %d Job %d finishes at CPU.", temp->priorityNum, temp->data);
			fprintf(fp, "\nAt time %d Job %d finishes at CPU.", temp->priorityNum, temp->data);

			//Generating a random number for the probability that the job exits the simulation
			int quit = randomNumber(10, 0);
			//If probability that job quits simulation
			if(quit < QUIT_PROB || quit == QUIT_PROB){
				printf("\nAt time %d Job %d exits.", temp->priorityNum, temp->data);
				fprintf(fp, "\nAt time %d Job %d exits.", temp->priorityNum, temp->data);
				//Dequeue job from CPU queue
				fDequeue(&cpuFront, &cpuRear);
			} 

			//Else the job will go to one of the disks if it does not quit
			else {
			//Statements checking which disk to put the job in

				//If the disk queues are of equal size, randomly pick one to put the job in
				if((fSize(&disk2Front)) == (fSize(&disk1Front))){	
					//Generating random number, 1 or 2
					int disk = randomNumber(2, 1);
					//Picked number 1
					if(disk == 1){
						//Sending job to Disk 1
						//Adding event of type job arrived at Disk 1 to the events queue
						pEnqueue(&eventsFront, &eventsRear, temp->data, temp->priorityNum, 5);
						//Dequeue job from CPU
						fDequeue(&cpuFront, &cpuRear);
					} else {
						//Sending job to Disk 2
						//Adding event of type job arrived at Disk 2 to the events queue
						pEnqueue(&eventsFront, &eventsRear, temp->data, temp->priorityNum, 6);
						//Dequeue job from CPU
						fDequeue(&cpuFront, &cpuRear);
					}
					
				} 
				//If Disk 2 has a longer queue put the job in Disk 1
				else if ((fSize(&disk2Front)) > (fSize(&disk1Front))){
					//Sending job to Disk 1
					//Adding event of type job arrived at Disk 1 to the events queue
					pEnqueue(&eventsFront, &eventsRear, temp->data, temp->priorityNum, 5);
					//Dequeue job from CPU
					fDequeue(&cpuFront, &cpuRear);
				} 
				//If Disk 1 has a longer queue put the job in Disk 2
				else{
					//Sending job to Disk 2
					//Adding event of type job arrived at Disk 2 to the events queue
					pEnqueue(&eventsFront, &eventsRear, temp->data, temp->priorityNum, 6);
					//Dequeue job from CPU
					fDequeue(&cpuFront, &cpuRear);
				}
			}
			//Dequeue the job that was completed from the CPU queue
			fDequeue(&cpuFront, &cpuRear);
		}
		//Else if event type is of type "Job Finished at Disk1"
		else if(temp->eventType == 2){
			//Increment number of jobs completed/finished at Disk1
			totalJobsFinDisk1++;

			printf("\nAt time %d Job %d finishes IO at Disk 1.", temp->priorityNum, temp->data);
			fprintf(fp, "\nAt time %d Job %d finishes IO at Disk 1.", temp->priorityNum, temp->data);
			int quit = randomNumber(10, 0);
			//If probability that job quits simulation
			if(quit < QUIT_PROB || quit == QUIT_PROB){
				printf("\nAt time %d Job %d exits.", temp->priorityNum, temp->data);
				fprintf(fp, "\nAt time %d Job %d exits.", temp->priorityNum, temp->data);
				//Dequeue the job that was completed from the Disk1 queue
				fDequeue(&disk1Front, &disk1Rear);
			} 
			//Else the job will go to back to cpu if it does not quit
			else {
				//Job is sent back to CPU queue
				//As a job is sent back to the CPU queue, an event is added to the event queue that is of type job arrived
				pEnqueue(&eventsFront, &eventsRear, temp->data, temp->priorityNum, 0);
				//Dequeue the job that was completed from the Disk1 queue
				fDequeue(&disk1Front, &disk1Rear);				
			}
			
		}
		//Else if event type is of type "Job Finished at Disk2"
		else if(temp->eventType == 3){
			//Increment number of jobs completed/finished at Disk2
			totalJobsFinDisk2++;
			
			printf("\nAt time %d Job %d finishes IO at Disk 2.", temp->priorityNum, temp->data);
			fprintf(fp, "\nAt time %d Job %d finishes IO at Disk 2.", temp->priorityNum, temp->data);
			int quit = randomNumber(10, 0);
			//If probability that job quits simulation
			if(quit < QUIT_PROB || quit == QUIT_PROB){
				printf("\nAt time %d Job %d exits.", temp->priorityNum, temp->data);
				fprintf(fp, "\nAt time %d Job %d exits.", temp->priorityNum, temp->data);
				//Dequeue the job that was completed from the Disk2 queue
				fDequeue(&disk2Front, &disk2Rear);

			} 
			//Else the job will go to back to cpu if it does not quit
			else {
				//Job is sent back to CPU queue
				//As a job is sent back to the CPU queue, an event is added to the event queue that is of type job arrived
				pEnqueue(&eventsFront, &eventsRear, temp->data, temp->priorityNum, 0);
				//Dequeue the job that was completed from the Disk2 queue
				fDequeue(&disk2Front, &disk2Rear);
			}
			
		}
		//Else if event is of type "Simulation Ended"
		else if(temp->eventType == 4){
			simFinished = 1;
			printf("\nAt time %d simulation finishes.\n", FIN_TIME);
			fprintf(fp, "\nAt time %d simulation finishes.\n", FIN_TIME);
		}
		
		//Else if event is of type "Job Arrived at Disk1"
		else if(temp->eventType == 5){
			printf("\nAt time %d Job %d arrives at Disk 1.", temp->priorityNum, temp->data);
			fprintf(fp, "\nAt time %d Job %d arrives at Disk 1.", temp->priorityNum, temp->data);
			fEnqueue(&disk1Front, &disk1Rear, temp->data, temp->priorityNum);
			int timeTaken = randomNumber(DISK1_MAX, DISK1_MIN);
			fEnqueue(&processTimeDisk1Front, &processTimeDisk1Rear, timeTaken, 0);
			runtime+= timeTaken;
			pEnqueue(&eventsFront, &eventsRear, temp->data, runtime, 2);
		}
	
		//Else if event is of type "Job Arrived at Disk2"
		else if(temp->eventType == 6){
			printf("\nAt time %d Job %d arrives at Disk 2.", temp->priorityNum, temp->data);
			fprintf(fp, "\nAt time %d Job %d arrives at Disk 2.", temp->priorityNum, temp->data);
			fEnqueue(&disk2Front, &disk2Rear, temp->data, temp->priorityNum);
			int timeTaken = randomNumber(DISK2_MAX, DISK2_MIN);
			fEnqueue(&processTimeDisk2Front, &processTimeDisk2Rear, timeTaken, 0);
			runtime+= timeTaken;
			pEnqueue(&eventsFront, &eventsRear, temp->data, runtime, 3);
		}		

		//Remove the event from the events queue after it has been handled
		pDequeue(&eventsFront, &eventsRear);
		
		//Get current size of each queue
		cpuQSize = fSize(&cpuFront);
		disk1QSize = fSize(&disk1Front);
		disk2QSize = fSize(&disk2Front);
		eventsQSize = pSize(&eventsFront);

		//Add current size of each queue to queue that holds all the sizes of each queue from every iteration of the loop
		fEnqueue(&cpuSizeQFront, &cpuSizeQRear, cpuQSize,0);
		fEnqueue(&disk1SizeQFront, &disk1SizeQRear, disk1QSize,0);
		fEnqueue(&disk2SizeQFront, &disk2SizeQRear, disk2QSize,0);
		fEnqueue(&eventsSizeQFront, &eventsSizeQRear, eventsQSize, 0);

	}

/*-------------------------- Simulation Ended -----------------------*/
	
	// Close the file opened for writing to log.txt
	fclose (fp);

/*--------------------- Calculations for statistics -----------------*/

//Maximum No. of Jobs and Average No. of Jobs in CPU, Disk1, Disk2 and Event queues
	//Averages
	//LoopCount is how many times the loop ran, which is equal to how many times we recorded the length of each queue
	//So dividing by that will give us the average queue length
	double cpuAveJobs = (sumQueue(&cpuSizeQFront)/loopCount);
	double disk1AveJobs = (sumQueue(&disk1SizeQFront)/loopCount);
	double disk2AveJobs = (sumQueue(&disk2SizeQFront)/loopCount);
	double eventsAveEvents = (sumQueue(&eventsSizeQFront)/fSize(&eventsSizeQFront));

	//Maximums
	int cpuMaxJobs = findMax(&cpuSizeQFront);
	int disk1MaxJobs = findMax(&disk1SizeQFront);
	int disk2MaxJobs = findMax(&disk2SizeQFront);
	int eventsMaxEvents = findMax(&eventsSizeQFront);

//Maximum Response Time and Average Response Time of servers (CPU, Disk1 and Disk2 queues)

	//Averages
	double cpuAvePTime = ((sumQueue(&processTimeCpuFront))/(fSize(&processTimeCpuFront)));
	double disk1AvePTime = ((sumQueue(&processTimeDisk1Front))/(fSize(&processTimeDisk1Front)));
	double disk2AvePTime = ((sumQueue(&processTimeDisk2Front))/(fSize(&processTimeDisk2Front)));

	//Maximums
	double cpuMaxPTime = findMax(&processTimeCpuFront);
	double disk1MaxPTime = findMax(&processTimeDisk1Front);
	double disk2MaxPTime = findMax(&processTimeDisk2Front);

//Number of jobs completed per unit of time for each server(CPU, Disk1 and Disk2 queues)
	double jobPerTimeCpu = totalJobsFinCpu/FIN_TIME;
	double jobPerTimeDisk1 = totalJobsFinDisk1/FIN_TIME;
	double jobPerTimeDisk2 = totalJobsFinDisk2/FIN_TIME;

/*--------------------- Writing statistics to stats.txt -------------*/

	//Create file pointer for writing to file
	FILE * f;
	//Open file for writing
	f = fopen ("stats.txt","w");

	fprintf(f, "\nAverage size of CPU queue: %.2f", cpuAveJobs);
	fprintf(f, "\nMax size of CPU queue: %d", cpuMaxJobs);
	fprintf(f, "\nAverage response time of CPU: %.2f", cpuAvePTime);
	fprintf(f, "\nMax response time of CPU: %.2f", cpuMaxPTime);
	fprintf(f, "\nNumber of jobs completed per unit of time at CPU: %.5f\n", jobPerTimeCpu);
	
	fprintf(f, "\nAverage size of Disk 1 queue: %.2f", disk1AveJobs);
	fprintf(f, "\nMax size of Disk 1 queue: %d", disk1MaxJobs);
	fprintf(f, "\nAverage response time of Disk 1: %.2f", disk1AvePTime);
	fprintf(f, "\nMax response time of Disk 1: %.2f", disk1MaxPTime);
	fprintf(f, "\nNumber of jobs completed per unit of time at Disk 1: %.5f\n", jobPerTimeDisk1);
	
	fprintf(f, "\nAverage size of Disk 2 queue: %.2f", disk2AveJobs);
	fprintf(f, "\nMax size of Disk 2 queue: %d", disk2MaxJobs);
	fprintf(f, "\nAverage response time of Disk 2: %.2f", disk2AvePTime);
	fprintf(f, "\nMax response time of Disk 2: %.2f", disk2MaxPTime);
	fprintf(f, "\nNumber of jobs completed per unit of time at Disk 2: %.5f\n", jobPerTimeDisk2);
	
	fprintf(f, "\nAverage size of Events queue: %.2f", eventsAveEvents);
	fprintf(f, "\nMax size of Events queue: %d\n", eventsMaxEvents);

	//Close the file opened for writing to stats.txt
	fclose (f);
}
