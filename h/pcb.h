#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "queue.h"

class Thread;
class Queue;

class PCB
{
public:
	PCB(Thread* myThread1, StackSize stackSize1, Time timeSlice1);
	~PCB();
	Thread* myThread;
	unsigned int sp, ss;
	volatile int stateOfThread;
	friend class IdleThread;
	Time timeSlice, timeLeft;
	int timeExpired;
	StackSize stackSize;
	unsigned* stack;
	static ID cnt;
	ID id;
	Queue* waitOnThisThread;
	void allocateMemoryForStack();
	static void wrapper();
};
#endif
