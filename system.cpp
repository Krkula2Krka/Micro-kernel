#include "system.h"
#include "schedule.h"
#include "ks.h"
#include <dos.h>
#include <stdlib.h>

volatile int System::willingly = 0;
void interrupt (*System::oldIR)(...) = 0;
volatile PCB* System::running = 0;
Thread* System::mainThread = 0;
IdleThread* System::idle = 0;
volatile unsigned int tsp, tss;
volatile PCB *nextToGetCPU;
Queue *System::allThreads = new Queue();
volatile Time System::timePassed = 0;

void tick();

void System::load() { 
#ifndef BCC_BLOCK_IGNORE
	lock;
	oldIR = getvect(0x08);
	setvect(0x08, timer);
	mainThread = new Thread(defaultStackSize, 1);
	mainThread->myPCB->stateOfThread = 1;
	running = (volatile PCB*)mainThread->myPCB;
	idle = new IdleThread();
	idle->start();
	unlock;
#endif
}

void System::unload() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	setvect(0x08, oldIR);
	delete mainThread;
	delete idle;
	delete allThreads;
	delete KernelSem::globalQueueSem;
	unlock;
#endif
}

void interrupt System::timer(...) {
	if(!willingly ) {
		tick(); 
		(*oldIR)();
		if (KernelSem::globalQueueSem != 0) KernelSem::timeSignal();
	} 
	if (!willingly && running->timeSlice != 0) timePassed++;
	if (!willingly && (timePassed < running->timeSlice || running->timeSlice == 0) ) return;
	willingly = 0;
	if (running->stateOfThread == 1 && running != idle->myPCB ) Scheduler::put((PCB*)running);
	nextToGetCPU = Scheduler::get();
	if (nextToGetCPU == 0) nextToGetCPU = idle->myPCB;
#ifndef BCC_BLOCK_IGNORE
	asm {
		mov tsp, sp
		mov tss, ss
	}
	running->sp = tsp;
	running->ss = tss;
	running = nextToGetCPU;
	tsp = running->sp;
	tss = running->ss;
	asm {
		mov sp, tsp
		mov ss, tss
	}
#endif
	timePassed = 0;
}
