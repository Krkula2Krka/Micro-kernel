#include "event.h"
#include "system.h"
#include "ke.h"


#ifndef BCC_BLOCK_IGNORE
Event::Event (IVTNo ivtNo) {
	lock;
	myImpl = new KernelEv(ivtNo);
	unlock;
}
#endif

Event::~Event () {
#ifndef BCC_BLOCK_IGNORE
	lock;
	delete myImpl;
	unlock;
#endif
}

void Event::wait() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	if (myImpl) myImpl->wait();
	unlock;
#endif
}

void Event::signal() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	if (myImpl) myImpl->signal();
	unlock;
#endif
}

#ifndef BCC_BLOCK_IGNORE
KernelEv::KernelEv (IVTNo ivtNo) {
	lock;
	threadThatCreatedMe = System::running->myThread;
	myKS = new KernelSem(0);
	IVTEntryNumber = ivtNo;
	IVTEntry::ivtEntries[IVTEntryNumber]->myKE = this;
	unlock;
}
#endif

KernelEv::~KernelEv () {
#ifndef BCC_BLOCK_IGNORE
	lock;
	IVTEntry::ivtEntries[IVTEntryNumber]->myKE = 0;
	delete myKS;
	unlock;
#endif
}

void KernelEv::wait() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	if (System::running->myThread == threadThatCreatedMe) {
		myKS->wait(0);
	}
	unlock;
#endif
}

void KernelEv::signal() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	if (myKS->val() == 1) {
		unlock;
		return;
	}
	myKS->signal();
	unlock;
#endif
}