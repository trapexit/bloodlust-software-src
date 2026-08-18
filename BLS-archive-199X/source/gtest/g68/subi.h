
struct subi:insttype
{
 subi():insttype("0000_0100zz_aaaaaa","subi",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {immtype=IMM_DSIZE;}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\tsub %s,%s \n",areg[ip.dsize],immreg[ip.dsize]);
  pushfx();

  genwrite(t,ip.dsize);
 }
} SUBI;



struct cmpi:insttype
{
 cmpi():insttype("0000_1100zz_aaaaaa","cmpi",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {immtype=IMM_DSIZE;}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\tcmp %s,%s \n",areg[ip.dsize],immreg[ip.dsize]);
  pushf();
 }
} CMPI;


//------------------------------------

//---------------------


//source is effective address
struct sub:insttype
{
 sub():insttype("1001_nnn0zz_aaaaaa","sub",(Dn|An|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR|aimm) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\tsub %s ptr [edi.D0+%d*4],%s \n",isize[ip.dsize],ip.n,areg[ip.dsize]);
  pushfx();
 }
} SUB;

//source is register
struct sub2:insttype
{
 sub2():insttype("1001_nnn1zz_aaaaaa","sub2",(iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\tsub %s,%s ptr [edi.D0+%d*4] \n",areg[ip.dsize],isize[ip.dsize],ip.n);
  pushfx();
  genwrite(t,ip.dsize);
 }
} SUB2;



//dest is ea
struct suba:insttype
{
 suba():insttype("1001_nnny11_aaaaaa","cmpa",(Dn|An|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR|aimm) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);
  if (ip.dsize==1) OUT("cwde");
  printf("\tsub [edi.A0+%d*4],eax\n",ip.n);
 }
} SUBA;



//-----------------



//source is effective address
struct cmp:insttype
{
 cmp():insttype("1011_nnn0zz_aaaaaa","cmp",(Dn|An|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR|aimm) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\tcmp %s ptr [edi.D0+%d*4],%s \n",isize[ip.dsize],ip.n,areg[ip.dsize]);
  pushf();
 }
} CMP;


//dest is effective address
struct cmpa:insttype
{
 cmpa():insttype("1011_nnny11_aaaaaa","cmpa",(Dn|An|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR|aimm) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);
  if (ip.dsize==1) OUT("cwde");
  skipf();
  printf("\tcmp [edi.A0+%d*4],eax\n",ip.n);
  pushf();
 }
} CMPA;




//------------------------------



struct subq:insttype
{
 subq():insttype("0101_ccc1zz_aaaaaa","subq",(Dn|An|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);
  skipf();
  printf("\tsub %s,%d \n",areg[ip.dsize],ip.c ? ip.c : 8);
  pushfx();
  genwrite(t,ip.dsize);
 }
} SUBQ;



//--------------------



//Dn<->Dn
struct subx:insttype
{
 subx():insttype("1001_mmm1zz_000nnn","subx",0) {}
 void gencore(int t,iparm &ip)
 {
  getreg("edx");
  skipf();
  getxflag();
  printf("\tsbb %s ptr [edi.D0+%d*4],%s\n",isize[ip.dsize],ip.m,dreg[ip.dsize]);
  pushfx();
 }
} SUBX ;



//-----------------------







