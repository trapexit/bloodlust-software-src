
struct andi:insttype
{
 andi():insttype("0000_0010zz_aaaaaa","andi",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {immtype=IMM_DSIZE;}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\tand %s,%s \n",areg[ip.dsize],immreg[ip.dsize]);
  pushf();

  genwrite(t,ip.dsize);
 }
} ANDI;

//----------------------------

struct andibccr:insttype
{
 andibccr():insttype("0000_001000_111100","andibccr",0) {immtype=IMM_BYTE;}
 void gencore(int t,iparm &ip)
 {
  OUT("popmflags");
  printf("\tand %s,%s \n",ccrreg,immreg[0]);
  OUT("pushmflags");
 }
} ANDIBCCR;


struct andiwsr:insttype
{
 andiwsr():insttype("0000_001001_111100","andiwsr",0) {immtype=IMM_WORD;}
 void gencore(int t,iparm &ip)
 {
  OUT("popmflags");
  printf("\tand %s,%s \n",srreg,immreg[1]);
  OUT("pushmflags");
 }
} ANDIWSR;

//--------------------


//source is effective andress
struct and:insttype
{
 and():insttype("1100_nnn0zz_aaaaaa","and",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR|aimm) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\tand %s ptr [edi.D0+%d*4],%s \n",isize[ip.dsize],ip.n,areg[ip.dsize]);
  pushf();
 }
} AND;

//source is register
struct and2:insttype
{
 and2():insttype("1100_nnn1zz_aaaaaa","and2",(iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\tand %s,%s ptr [edi.D0+%d*4] \n",areg[ip.dsize],isize[ip.dsize],ip.n);
  pushf();
  genwrite(t,ip.dsize);
 }
} AND2;





