// to submit

#include "ece150-proj3-provided.h"

#include <math.h>
#include <iostream>
#include <string.h>
using namespace std;

// recomendations
/* better way of "copying" the linked list, would require use of new to copy nodes into active queue list
current method is inefficient because it does not allow the comparison of ptr->next == null
*/


timestamp maxClientDelay(uri *, request *);
int stampToSeconds(const timestamp &);
timestamp secondsToStamp(int);
int getPri(char *, uri *);
int getSize(char *, uri *);

/*
void printTimestamp(timestamp t) {
    if(t.hh < 10) cout << "0";
    cout << t.hh << ":";
    if(t.mm < 10) cout << "0";
    cout << t.mm << ":";
    if(t.ss < 10) cout << "0";
    cout << t.ss;
}
*/

class queueList {
	friend timestamp maxClientDelay(uri *, request *);
	
	public:
		queueList();
		void enqueue(request *);
		void dequeue(request *);
		//void printQueue(uri *);
	private:
		request *firstPtr;
		request *lastPtr;
};

queueList::queueList(){
	firstPtr = NULL;
	lastPtr = NULL;
}

/*
void queueList::printQueue(uri *uris){
	
	if (firstPtr == NULL){
		//cout << "printQueue called, but the queue is actually empty" << endl;
		return;
	}
	
	request *current = firstPtr;
	while (current != lastPtr){
		cout << current->name << " " << getPri(current->name, uris) << endl;
		current = current->next;
	}
//	cout << lastPtr->name << " " << getPri(lastPtr->name, uris) << endl;
	//cout << "\n\n\n";
}
*/

		
void queueList::enqueue(request * ptr){
	if (firstPtr == NULL){ // if it's empty
		firstPtr = ptr;
		lastPtr = ptr;
		
		//lastPtr->next = NULL; this line is not allowed as it breaks the linked list...
		
	}else{
		// add it to the front...
		/*
		request *temp = firstPtr;
		firstPtr = ptr;
		firstPtr->next = temp;
		*/
		lastPtr->next = ptr;
		lastPtr = ptr;
		
		
	}
}

void queueList::dequeue(request *ptr){
	//cout << "The dequeue function has been called on pointer " << ptr << endl;
	// removes a node, the arg is the pointer to the client request that is to be serviced (removed)
	// its a little tricky to remove a node thats in the middle of a list
	if (firstPtr == NULL){
		cerr << "the queue is emty, cannot deqeueu" << endl;
	}else if (firstPtr == lastPtr){ // if there's only one element in the linked list
		//cout << "The the one and only element in the queue has been removed, the queue is now empty " << endl;
		firstPtr = NULL;
		lastPtr = NULL;
	}else{
		
		//cout << " There's more than one element in the list" << endl;
		// cycle through the linked list, until we have a match for the node to be removed
		// check if it's the first element
		if (firstPtr == ptr){
			//cout << "The first element is removed " << endl;
			firstPtr = ptr->next;
			return;
		}
		
		request *currentPtr = firstPtr;
		do {
			if (currentPtr->next == ptr){ // if its a match
				
				//cout << " it's a match, remove this node, but first chekc if it's the last element " << endl;
				
				if (currentPtr->next == lastPtr){
					//cout << " the element at the end was removed" << endl;
					
					lastPtr = currentPtr; // makes sense
					
					// currentPtr->next = NULL;
					
				}else{
					//cout << " normal case removal" << endl;
					currentPtr->next = ptr->next; // equivalent to re assinging it with currentPtr->next->next
					lastPtr = currentPtr->next;
					
				}
				
				break; // 
				
			}
			
			currentPtr = currentPtr->next;
		}while(currentPtr->next != 0);
		
	}
	
	
	return;
	
}


timestamp secondsToStamp(int seconds){
	// converts seconds to a time stamp
	timestamp output;
	int diff = seconds;
	
	output.hh = diff / (60*60);
	diff = diff % (60*60);
	output.mm = diff / 60;
	output.ss = diff % 60;
	return output;
}


 int stampToSeconds(const timestamp &stamp){
	int output = stamp.ss + stamp.mm*60 + stamp.hh*60*60;
	return output;
}


int getPri(char *nameUri, uri *data){
	
	// given the name of a uri, it cycles thru the uri database until a match
	// returns the pri of the matching uri
	
	uri *currentUriPtr = data;
	
	while(currentUriPtr != 0){
		if (strcmp(currentUriPtr->name, nameUri) == 0){
			// then its a match so extract is pri value
			return currentUriPtr->priority;
		}			
		currentUriPtr = currentUriPtr->next;
	}
	return 0;
}

int getSize(char *nameUri, uri *data){

	uri *currentUriPtr = data;
	
	while(currentUriPtr != 0){
		if (strcmp(currentUriPtr->name, nameUri) == 0){
			// then its a match so extract is pri value
			return currentUriPtr->size;
		}			
		currentUriPtr = currentUriPtr->next;
	}
	return 0;
}


timestamp maxClientDelay(uri *dataBasePtr, request *listPtr){
	
	int time = 0; // in seconds
	request *currentReqPtr = listPtr; // this pointer is used to cycle through the FULL list of requests
	queueList activeQueue; // initalize this 
	int largestQueueTime = 0;
	
	//cout << "Starting the simulator \n" << endl;
	
	while (time < 24*60*60 || currentReqPtr != NULL || activeQueue.firstPtr != NULL){
		// add requests with timestamp less than the current time to my Q
		// loop through the request list and find the appropriate requests
		// add them to my quee
		
		//cout << "Adding requests with time stamp less than current time: " << time << endl;
		// this loop is being executed continuously for some reason....
	
		while(currentReqPtr != NULL && stampToSeconds(currentReqPtr->t) <= time){
			
			activeQueue.enqueue(currentReqPtr);
			//cout << "enqueue function called, request added to queue " << endl;
			//cout << "its time stamp is (in seconds) " << stampToSeconds(currentReqPtr->t) << endl;
			currentReqPtr = currentReqPtr->next;
			//cout << "currentReqPtr is updated to " << currentReqPtr << endl; 
		}
		
		//cout << "\n done adding requests to queue \n" << endl;
		//activeQueue.printQueue(dataBasePtr);
		// if theres one or more pending request then dequeue
		// go through activeQueue lisr and remove the node with the largest pri
		
		if (activeQueue.firstPtr != 0){ 
			
			//cout << "the queue list is NOT empty; so service the request with the highest pri" << endl;
			//cout << " so cycle through the activeQueue" << endl;
				
			request *currentQueuePtr = activeQueue.firstPtr; // creates a pointer to the first node
			request *largestPtr = activeQueue.firstPtr; // this keeps track of the node with the highest pri
			
			
			while (currentQueuePtr != activeQueue.lastPtr){
				// request structure contains a string to the uri
				// so cycle thru the uri list until a match occurs
				
				if (getPri(largestPtr->name, dataBasePtr) < getPri(currentQueuePtr->name, dataBasePtr)){
					// keep the larger 
					largestPtr = currentQueuePtr;
					
				}
				currentQueuePtr = currentQueuePtr->next; // increment pointer
				//cout << "currentQueuePtr has been incremented to " << currentQueuePtr << endl;
			}
			
			if (getPri(largestPtr->name, dataBasePtr) < getPri(activeQueue.lastPtr->name, dataBasePtr)){
				// keep the larger 
				largestPtr = currentQueuePtr;
				
			}
			
			
			// the largest client request with the highest pri has been identified, now remove it
			// this will depend on whether or not it is the first, middle or last
			
			activeQueue.dequeue(largestPtr);
			//activeQueue.printQueue(dataBasePtr);
			
			//cout << "The request with the largest pri has been dequeued \n" << endl;
			// get the current time for when it's dequeued
			int queueTime = time - stampToSeconds(largestPtr->t); // keep track of the largest
			//cout << "It's queue time was: " << queueTime << endl;
			
			if (queueTime > largestQueueTime){
				largestQueueTime = queueTime;
				//cout << " The current largest queue time is " << largestQueueTime << endl;
			}
			
			double delay = getSize(largestPtr->name, dataBasePtr)*0.8; // in seconds...
			int timeProgressed = round(delay); // in seconds
			time += timeProgressed;
			//cout << "Time is progressed by: " << timeProgressed << endl << endl;
			
		}else{
			/*
			cout << " the queue list is empty, so progress time " << endl;
			// progress time to the timestamp of the earliest new client request
			cout << "currentReqPtr is " << currentReqPtr<< endl;
			cout << "the address of time is " << &time << endl;
			cout << " the value of time is " << time << endl;
			*/
			
			// this line causes it to crash as currentReqPtr = NULL,
			if (currentReqPtr == NULL){
				//cout << "Exiting program" << endl;

				goto end;
			}
			
			
			time = stampToSeconds(currentReqPtr->t); 
			//cout << "the time has been progressed to: " <<  time << endl;
			
		
			// if all the requests have been served then just end it
		}	
		
		
	}
	
	end:
	timestamp output = secondsToStamp(largestQueueTime);
	//cout << "My answer is " << stampToSeconds(output);
	//cerr << "wtf is the ";
	return output; 
}		

