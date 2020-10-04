#ifndef KS_H_
#define KS_H_

#include "semaphor.h"
#include "queue.h"

class KernelSem {
public:
	KernelSem(int init=1);
	virtual ~KernelSem();
	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n=0);
	int val() const;
	static Queue *globalQueueSem;
	static void timeSignal();
private:
	void waitingQueueSignal();
	Queue *waiting, *blocked;
	int value;
};
#endif
