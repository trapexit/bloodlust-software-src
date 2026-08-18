
struct eori:insttype
{
 eori():insttype("0000_1010zz_aaaaaa","eori",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {immtype=IMM_DSIZE;}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\txor %s,%s ;modify\n",areg[ip.dsize],immreg[ip.dsize]);
  pushf();

  genwrite(t,ip.dsize);
 }
} EORI;

//----------------------------

struct eoribccr:insttype
{
 eoribccr():insttype("0000_101000_111100","eoribccr",0) {immtype=IMM_BYTE;}
 void gencore(int t,iparm &ip)
 {
  OUT("popmflags");
  printf("\txor %s,%s ;modify\n",ccrreg,immreg[0]);
  OUT("pushmflags");
 }
} EORIBCCR;


struct eoriwsr:insttype
{
 eoriwsr():insttype("0000_101001_111100","eoriwsr",0) {immtype=IMM_WORD;}
 void gencore(int t,iparm &ip)
 {
  OUT("popmflags");
  printf("\txor %s,%s ;modify\n",srreg,immreg[1]);
  OUT("pushmflags");
 }
} EORIWSR;


//--------------------------


//source is register
struct eor:insttype
{
 eor():insttype("1011_nnn1zz_aaaaaa","eor",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\txor %s,%s ptr [edi.D0+%d*4]\n",areg[ip.dsize],isize[ip.dsize],ip.n);
  pushf();
  genwrite(t,ip.dsize);
 }
} EOR;


