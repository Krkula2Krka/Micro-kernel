#include "system.h"
#include "idlethr.h"
#include "pcb.h"

IdleThread::IdleThread(): Thread(2048, 1) {}

void IdleThread::run() {
	while(1);
}

void IdleThread::start() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	myPCB->stateOfThread = 1;
	myPCB->allocateMemoryForStack();
	unlock;
#endif
}
