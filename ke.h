#ifndef KE_H_
#define KE_H_

#include "event.h"
#include "ivtentry.h"
#include "ks.h"

typedef unsigned char IVTNo;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();
private:
	KernelSem *myKS;
	int IVTEntryNumber;
	Thread *threadThatCreatedMe;
};
#endif
