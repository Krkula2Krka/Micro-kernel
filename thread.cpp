#include "system.h"
#include "pcb.h"
#include "thread.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <stdlib.h>

#ifndef BCC_BLOCK_IGNORE
Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock;
	myPCB = new PCB(this, stackSize, timeSlice);
	unlock;
}
#endif

Thread::~Thread() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	waitToComplete();
	delete myPCB;
	unlock;
#endif
}

void Thread::start() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	if (myPCB->stateOfThread != 0) {
		unlock;
		return;
	}
	myPCB->stateOfThread = 1;
	myPCB->allocateMemoryForStack();
	Scheduler::put(myPCB);
	unlock;
#endif
}

void Thread::waitToComplete() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	if (myPCB->stateOfThread == 2 || myPCB->stateOfThread == 0) {
		unlock;
		return;
	}
	if (myPCB == (PCB*)System::running) {
		unlock;
		return;
	}
	if (this == System::mainThread) {
		unlock;
		return;
	}
	if (this == System::idle) {
		unlock;
		return;
	}
	System::running->stateOfThread = -1;
	myPCB->waitOnThisThread->putPCB((PCB*)System::running);
	dispatch();
	unlock;
#endif
}

#ifndef BCC_BLOCK_IGNORE
ID Thread::getId() {
	return myPCB->id;
}
#endif

#ifndef BCC_BLOCK_IGNORE
ID Thread::getRunningId() {
	lock;
	ID ret = System::running->id;
	unlock;
	return ret;
}
#endif

#ifndef BCC_BLOCK_IGNORE
Thread *Thread::getThreadById(ID id) {
	lock;
	Thread *ret = System::allThreads->getThreadById(id);
	unlock;
	return ret;
}
#endif


void dispatch() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	System::willingly = 1;
	System::timer();
	System::willingly = 0;
	unlock;
#endif
}

int PCB::cnt = 0;

#ifndef BCC_BLOCK_IGNORE
PCB::PCB(Thread* myThread1, StackSize stackSize1, Time timeSlice1) {
	lock;
	stateOfThread = 0;
	id = cnt++;
	System::allThreads->putPCB(this);
	if (stackSize > 65535) stackSize = 65535;
	stackSize = stackSize1;
	myThread = myThread1;
	stack = 0;
	timeSlice = timeSlice1;
	waitOnThisThread = new Queue();
	unlock;
}
#endif

PCB::~PCB() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	delete waitOnThisThread;
	delete stack;
	unlock;
#endif
}

void PCB::allocateMemoryForStack() {
#ifndef BCC_BLOCK_IGNORE
  	lock;
  	stackSize= stackSize/sizeof(unsigned);
  	stack = new unsigned[stackSize];
  	stack[stackSize-1] = 0x200;
  	stack[stackSize-2] = FP_SEG(&wrapper);
  	stack[stackSize-3] = FP_OFF(&wrapper);
	sp = FP_OFF(stack+stackSize-12);
	ss = FP_SEG(stack+stackSize-12);
	unlock;
#endif
}

void PCB::wrapper() {
	System::running->myThread->run();
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	System::running->stateOfThread = 2;
	PCB* temp = System::running->waitOnThisThread->getPCB();
	while (temp) {
		temp->stateOfThread = 1;
		Scheduler::put(temp);
		temp = System::running->waitOnThisThread->getPCB();
	}
	dispatch();
}