#include <stdlib.h>
#include "queue.h"
#include "system.h"


#ifndef BCC_BLOCK_IGNORE
Queue::Queue() {
	lock;
	first = 0;
	unlock;
}
#endif

Queue::~Queue() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	Elem* temp;
	while( first != 0 )	{
		temp = first;
		first = first->next;
		delete temp;
	}
	unlock;
#endif
}

#ifndef BCC_BLOCK_IGNORE
PCB* Queue::getPCB() {
	lock;
	if (first == 0) {
		unlock;
		return 0;
	}
	Elem* temp = first;
	first = first->next;
	PCB *ret = temp->pcb;
	delete temp;
	unlock;
	return ret;
}
#endif

void Queue::putPCB(PCB* pcb) {
#ifndef BCC_BLOCK_IGNORE
	lock;
	Elem* prev = first;
	while(prev !=0 && prev->next != 0) prev = prev->next;
	Elem* elem = new Elem();
	elem->next = 0;
	elem->pcb = pcb;
	if(first == 0) first = elem;
	else prev->next = elem;
	unlock;
#endif
}

void Queue::putKS(KernelSem* ks) {
#ifndef BCC_BLOCK_IGNORE
	lock;
	Elem* prev = first;
	while(prev !=0 && prev->next != 0) prev = prev->next;
	Elem* elem = new Elem();
	elem->next = 0;
	elem->ks = ks;
	if(first == 0) first = elem;
	else prev->next = elem;
	unlock;
#endif
}

#ifndef BCC_BLOCK_IGNORE
Thread *Queue::getThreadById(ID id) {
	Thread *ret = 0;
	lock;
	for (Elem *temp=first; temp; temp=temp->next) {
		if (temp->pcb->id == id) {
			ret = temp->pcb->myThread;
			break;
		}
	}
	unlock;
	return ret;
}
#endif
