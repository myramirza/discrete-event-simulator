#include <stdio.h>
#include <stdlib.h>

struct fNode{
	int data;
	int time;
	struct fNode* next;
};

//Function to add an element to the queue
void fEnqueue(struct fNode** front, struct fNode** rear, int data, int time){
	struct fNode* temp = (struct fNode*)malloc(sizeof(struct fNode*));
	temp->data = data;
	temp->time = time;
	temp->next = NULL;
	
	//Two cases for insertion.
	//Case 1: Queue is empty
	if((*front) == NULL && (*rear) == NULL){
		(*front) = temp;
		(*rear) = temp;
		return;
	}

	//Case 2: Queue is not empty
	(*rear)->next = temp;
	(*rear) = temp;
}

//Function to delete an element from the queue
void fDequeue(struct fNode** front, struct fNode** rear){
	//We need to create temp so we don't lose front as it is our only reference for the queue. So we create a temp and copy front into it and use temp instead
	struct fNode* temp = (*front);

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

//Function to check if the queue is empty
int fIsEmpty(struct fNode** front){
	return (*front) == NULL;
}

//Function to see the element at the front of the queue
struct fNode* fPeek(struct fNode** front){
	return (*front);
}

double fSize(struct fNode** front ){
	struct fNode* temp = (struct fNode*)malloc(sizeof(struct fNode*));
	temp = (*front);
	int count = 0;
	while(temp != NULL){
		count++;
		temp = temp->next;
	}
	return count;
}

void fPrint(struct fNode** front){
	struct fNode* temp = (struct fNode*)malloc(sizeof(struct fNode*));
	temp = (*front);
	printf("The list is: ");
	while(temp != NULL){
		printf(" %d", temp->data);
		temp = temp->next;
	}
	printf("\n");
}

double sumQueue(struct fNode** front){
	int sum = 0;
	struct fNode* temp = (struct fNode*)malloc(sizeof(struct fNode*));
	temp = (*front);
	while(temp != NULL){
		sum += temp->data;
		temp = temp->next;
	}
	return sum;
}

int findMax(struct fNode** front){
	int max;
	struct fNode* temp = (struct fNode*)malloc(sizeof(struct fNode*));
	temp = (*front);
	max = temp->data;
	while(temp != NULL){
		if(max < temp->data){
			max = temp->data;
		}
		temp = temp->next;
	}
	return max;
}


