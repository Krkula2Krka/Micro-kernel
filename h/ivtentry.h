#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

#define PREPAREENTRY(ivtno,old) \
void interrupt routine##ivtno(...); \
IVTEntry ent##ivtno(ivtno,routine##ivtno); \
void interrupt routine##ivtno(...) { \
	if (old) ent##ivtno.callOldIR(); \
	ent##ivtno.signal(); \
	dispatch(); \
}

class IVTEntry {
public:
	friend class KernelEv;
	static IVTEntry* ivtEntries[256];
	unsigned ivtNo;
	KernelEv* myKE;
	void interrupt (*oldIR)(...);
	IVTEntry(int, void interrupt (*)(...));
   ~IVTEntry();
	void signal();
	void callOldIR();
};
#endif
