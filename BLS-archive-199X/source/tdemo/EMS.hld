typedef struct memptr
{
int      pages;  //if zero, conventional
char far *ptr;
};


extern "C"
{
void InitEMSMem();
void TerminateEMSMem();
char far *GetPtr(memptr m);
int  AllocEMSMem(unsigned bytes,memptr *m);
void FreeEMSMem();
unsigned long EMSFree();
unsigned long EMSTotal();

}

extern unsigned long baseptr;
