
struct neg:insttype
{
 neg():insttype("0100_0100zz_aaaaaa","neg",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\tneg %s \n",areg[ip.dsize]);
  pushfx();

  genwrite(t,ip.dsize);
 }
} NEG;


//??????????????????
struct negx:insttype
{
 negx():insttype("0100_0000zz_aaaaaa","negx",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  getxflag();
  printf("\tneg %s    \n",areg[ip.dsize]);
  printf("\tsbb %s,0 \n",areg[ip.dsize]);
  pushfx();

  genwrite(t,ip.dsize);
 }
} NEGX;


//-------------
struct clr:insttype
{
 clr():insttype("0100_0010zz_aaaaaa","clr",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {}
 void gencore(int t,iparm &ip)
 {
  skipf();
  printf("\txor eax,eax\n");
  pushf();

  genwrite(t,ip.dsize);
 }
} CLR;



//--------------------
struct not:insttype
{
 not():insttype("0100_0110zz_aaaaaa","not",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\tnot %s \n",areg[ip.dsize]);
  pushf();

  genwrite(t,ip.dsize);
 }
} NOT;


//--------------------


struct swap:insttype
{
 swap():insttype("0100_100001_000nnn","swap",0)
  {}
 void gencore(int t,iparm &ip)
 {
  getreg("edx");
  skipf();
  OUT("ror edx,16 ");
  OUT("test edx,edx");
  pushf();
  setreg("edx");
 }
} SWAP ;



//--------------------


struct pea:insttype
{
 pea():insttype("0100_100001_aaaaaa","pea",(iAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR) )
  {naturalize=0;}
 void gencore(int t,iparm &ip)
 {
  OUT("mov eax,edx ; get addr");
  pushd("eax");
 }
} PEA;


//---------------------




struct ext:insttype
{
 ext():insttype("0100_10001y_000nnn","ext",0)
  {}
 void gencore(int t,iparm &ip)
 {
  OUT("mov edx,eax");
  getreg("eax");

  skipf();
  if (ip.dsize==1) //byte->word
   {
    OUT("cbw");
    OUT("test ax,ax");
   } else
  if (ip.dsize==2) //word->dword
   {
    OUT("cwde");
    OUT("test eax,eax");
   } else printf("FUCKED\n");
  pushf();

  OUT("mov [edi.D0+edx*4],eax");
 }
} EXT ;

//-----------


struct tst:insttype
{
 tst():insttype("0100_1010zz_aaaaaa","tst",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize);

  skipf();
  printf("\ttest %s,%s \n",areg[ip.dsize],areg[ip.dsize]);
  pushf();
 }
} TST;




//-----------




struct tas:insttype
{
 tas():insttype("0100_101011_aaaaaa","tas",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) )
  {}
 void gencore(int t,iparm &ip)
 {
  ip.dsize=0;
  genread(t,0);

  skipf();
  OUT("test al,al ;test");
  pushf();
  OUT("or al,80h ;set msb");
  genwrite(t,0);
 }
} TAS;




//-------------------

struct lea:insttype
{
 lea():insttype("0100_nnn111_aaaaaa","lea",(iAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR) )
  {naturalize=0;}
 void gencore(int t,iparm &ip)
 {
  printf("\tmov [edi.A0+%d*4],edx ; get addr\n",ip.n);
 }
} LEA;




//------------------

//Dn<->Dn
struct exg:insttype
{
 exg():insttype("1100_mmm101_000nnn","exg",0) {}
 void gencore(int t,iparm &ip)
 {
  getreg("edx");
  printf("\tmov ebx,[edi.D0+%d*4]\n",ip.m);
  setreg("ebx");
  printf("\tmov [edi.D0+%d*4],edx\n",ip.m);
 }
} EXG ;


//An<->An
struct exg2:insttype
{
 exg2():insttype("1100_mmm101_001nnn","exg2",0) {}
 void gencore(int t,iparm &ip)
 {
  getareg("edx");
  printf("\tmov ebx,[edi.A0+%d*4]\n",ip.m);
  setareg("ebx");
  printf("\tmov [edi.A0+%d*4],edx\n",ip.m);
 }
} EXG2 ;


//An<->Dn
struct exg3:insttype
{
 exg3():insttype("1100_mmm110_001nnn","exg3",0) {}
 void gencore(int t,iparm &ip)
 {
  getareg("edx");
  printf("\tmov ebx,[edi.D0+%d*4]\n",ip.m);
  setareg("ebx");
  printf("\tmov [edi.D0+%d*4],edx\n",ip.m);
 }
} EXG3 ;


