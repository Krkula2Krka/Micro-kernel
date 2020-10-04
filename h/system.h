#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "pcb.h"
#include "thread.h"
#include "idlethr.h"

#define lock asm{\
 	pushf;\
 	cli;\
}
#define unlock asm popf

class Queue;

class System {
public:
	static void load();
	static void unload();
	friend class Thread;
	friend class PCB;
	friend class Queue;
	static Queue *allThreads;
	static Thread* mainThread;
	static IdleThread* idle;
	static volatile PCB* running;
	static volatile int willingly;
	static volatile Time timePassed;
	static void interrupt (*oldIR)(...);
	static void interrupt timer(...);
};

#endif
