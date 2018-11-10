#include <stdio.h>
#include <stdlib.h>

struct pNode {
	int data;
	int eventType;
	int priorityNum; //priority is in ascending order i.e 1 has higher priority than 2
	struct pNode* next; //storing address of next node
};

//Function to print the queue
void pPrint(struct pNode** front){
	struct pNode* temp = (struct pNode*)malloc(sizeof(struct pNode*));
	temp = (*front);
	printf("The list is: ");
	while(temp != NULL){
		printf("\n Job no: %d", temp->data);
		printf("\n Event type: %d", temp->eventType);
		printf("\n Time: %d", temp->priorityNum);
		temp = temp->next;
	}
	printf("\n");
}

//Function to add an element to the queue
void pEnqueue(struct pNode** front, struct pNode** rear, int data, int priorityNum, int eventType){
	struct pNode* temp = (struct pNode*)malloc(sizeof(struct pNode*));
	temp->data = data;
	temp->eventType = eventType;
	temp->priorityNum = priorityNum;
	temp->next = NULL;

	struct pNode* start = (struct pNode*)malloc(sizeof(struct pNode*));
	start = (*front);
	
	//Two cases for insertion.
	//Case 1: Queue is empty
	if((*front) == NULL && (*rear) == NULL){
		(*front) = temp;
		(*rear) = temp;
		return;
	}

	//Case 2: Queue is not empty
	//Case 2a: New element priority is higher than front
	else if ((*front)->priorityNum > priorityNum) { 
  
        	// Insert new element as the new front of the queue
        	temp->next = (*front); 
        	(*front) = temp; 
    	} 
	//Case 2b: New node priority is lower than front
    	else { 
		// Traverse the list to find the appropriate position based on priority number 
		// If priority of new element is same as a previous element in the queue then it is placed based on FIFO
		while (start->next != NULL && 
		       (start->next->priorityNum < priorityNum || start->next->priorityNum < priorityNum)) { 
		    start = start->next; 
        } 
  
        // Now reached the appropriate destination
        temp->next = start->next; 
        start->next = temp; 
    } 
}

//Function to delete an element from the queue
void pDequeue(struct pNode** front, struct pNode** rear){
	//We need to create temp so we don't lose front as it is our only reference for the queue. So we create a temp and copy front into it and use temp instead
	struct pNode* temp = (*front);

	//Three cases for deletion
	//Case 1: Queue is empty
	if((*front) == NULL && (*rear) == NULL){
		return;
	} 

	//Case 2: There is only 1 element in the queue
	else if((*front) == (*rear)){
		(*front) = NULL;
		(*rear) = NULL;
		free(temp); //Anything in dynamic memory has to be explicitly freed	
	}
	
	//Case 2: Queue is not empty
	else{
		(*front) = (*front)->next;
		free(temp);
	}
}

//Function to see the element at the front of the queue
struct pNode* pPeek(struct pNode** front){
	return (*front);
}

//Function to see if the queue is empty
int pIsEmpty(struct pNode** front){
	return (*front) == NULL;
}

int pSize(struct pNode** front ){
	struct pNode* temp = (struct pNode*)malloc(sizeof(struct pNode*));
	temp = (*front);
	int count = 0;
	while(temp != NULL){
		count++;
		temp = temp->next;
	}
	return count;
}









