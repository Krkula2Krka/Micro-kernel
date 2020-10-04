#include "ivtentry.h"
#include <dos.h>
#include "system.h"
#include "ke.h"

IVTEntry* IVTEntry::ivtEntries[256] = {0};

#ifndef BCC_BLOCK_IGNORE
IVTEntry::IVTEntry(int num, void interrupt (*newIR)(...)) {
	lock;
	myKE = 0;
	ivtNo = num;
	ivtEntries[num] = this;
	oldIR = getvect(num);
	setvect(num, newIR);
	unlock;
}
#endif

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	oldIR();
	setvect(ivtNo, oldIR);
	unlock;
#endif
}

void IVTEntry::signal() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	if (myKE) myKE->signal();
	unlock;
#endif
}

void IVTEntry::callOldIR() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	oldIR();
	unlock;
#endif
}

