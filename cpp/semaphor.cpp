#include "semaphor.h"
#include "system.h"
#include "ks.h"
#include "SCHEDULE.H"

#ifndef BCC_BLOCK_IGNORE
Semaphore::Semaphore (int init) {
	lock;
	myImpl = new KernelSem(init);
	unlock;
}
#endif

Semaphore::~Semaphore () {
#ifndef BCC_BLOCK_IGNORE
	lock;
	delete myImpl;
	unlock;
#endif
}

#ifndef BCC_BLOCK_IGNORE
int Semaphore::wait(Time maxTimeToWait) {
	lock;
	int ret = myImpl->wait(maxTimeToWait);
	unlock;
	return ret;
}
#endif

#ifndef BCC_BLOCK_IGNORE
int Semaphore::signal(int n) {
	lock;
	int ret = myImpl->signal(n);
	unlock;
	return ret;
}
#endif

#ifndef BCC_BLOCK_IGNORE
int Semaphore::val() const {
	lock;
	int ret = myImpl->val();
	unlock;
	return ret;
}
#endif


Queue *KernelSem::globalQueueSem = new Queue();

#ifndef BCC_BLOCK_IGNORE
KernelSem::KernelSem (int init) {
	lock;
	value = init;
	waiting = new Queue();
	blocked = new Queue();
	globalQueueSem->putKS(this);
	unlock;
}
#endif

KernelSem::~KernelSem () {
#ifndef BCC_BLOCK_IGNORE
	lock;
	delete waiting;
	delete blocked;
	unlock;
#endif
}

#ifndef BCC_BLOCK_IGNORE
int KernelSem::wait(Time maxTimeToWait) {
	int ret = 1;
	lock;
	if (--value < 0) {
		System::running->timeExpired = 0;
		System::running->stateOfThread = -1;
		System::running->timeLeft = maxTimeToWait;
		if (maxTimeToWait > 0) waiting->putPCB((PCB*)System::running);
		else blocked->putPCB((PCB*)System::running);
		dispatch();
		if (System::running->timeExpired == 1) ret = 0;
	}
	unlock;
	return ret;
}
#endif

#ifndef BCC_BLOCK_IGNORE
int KernelSem::signal(int n) {
	int ret = 0;
	lock;
	if (n == 0) {
		if (++value <= 0) {
			PCB *pcb = blocked->getPCB();
			if (pcb == 0) pcb = waiting->getPCB();
			pcb->stateOfThread = 1;
			Scheduler::put(pcb);
		}
	}
	if (n > 0) {
		int unblocked = 0;
		int i=0;
		while (i<n) {
			if (++value <= 0) {
				unblocked++;
				PCB *pcb = blocked->getPCB();
				if (pcb == 0) pcb = waiting->getPCB();
				pcb->stateOfThread = 1;
				Scheduler::put(pcb);
			}
			i++;
		}
		ret = unblocked;
	}
	if (n < 0) ret = n;
	unlock;
	return ret;
}
#endif

#ifndef BCC_BLOCK_IGNORE
int KernelSem::val() const {
	lock;
	int ret = value;
	unlock;
	return ret;
}
#endif

void KernelSem::timeSignal() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	Elem *temp = globalQueueSem->first;
	while (temp) {
		temp->ks->waitingQueueSignal();
		temp = temp->next;
	}
	unlock;
#endif
}

void KernelSem::waitingQueueSignal() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	Elem *temp = waiting->first;
	Elem *prev = 0, *old = 0;
	while (temp != 0) {
		if (--temp->pcb->timeLeft > 0) {
			prev = temp;
			temp = temp->next;
		}
		else {
			value++;
			temp->pcb->timeExpired = 1;
			temp->pcb->stateOfThread = 1;
			Scheduler::put(temp->pcb);
			old = temp;
			temp = temp->next;
			if (!prev) waiting->first = temp;
			else prev->next = temp;
			delete old;
		}
	}
	unlock;
#endif
}
