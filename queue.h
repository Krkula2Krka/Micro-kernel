#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "thread.h"
#include "pcb.h"

class PCB;
class KernelSem;

class Elem {
public:
	PCB* pcb;
	KernelSem *ks;
	Elem* next;
};

class Queue {
public:
	void putPCB(PCB* pcb);
	PCB* getPCB();
	void putKS(KernelSem *sem);
	Thread *getThreadById(ID id);
	Queue();
	~Queue();
	friend class KernelSem;
private:
	Elem* first;
};


#endif
