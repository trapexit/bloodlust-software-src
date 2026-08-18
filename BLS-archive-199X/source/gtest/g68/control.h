//---------
struct illegal:insttype
{
 illegal():insttype("0100_101011_111100","illegal",0)  {}
 void gencore(int t,iparm &ip)
 {
//  OUT("g68int V_ILLEGALINST");
  OUT("jmp badopcode");
 }
} ILLEGAL;


//---------

struct trap:insttype
{
 trap():insttype("0100_111001_00cccc","trap",0)  {}
 void gencore(int t,iparm &ip)
 {
  OUT("mov al,bh");
  OUT("and eax,1111b");
  OUT("add eax,V_TRAP");
  OUT("g68int eax");
 }
} TRAPF;

//---------

struct reset:insttype
{
 reset():insttype("0100_111001_110000","reset",0)  {}
 void gencore(int t,iparm &ip) {}
} RESET;


//---------

struct nop:insttype
{
 nop():insttype("0100_111001_110001","nop",0)  {}
 void gencore(int t,iparm &ip) {}
} NOP;



//---------

struct stop:insttype
{
 stop():insttype("0100_111001_110010","stop",0)
  {immtype=IMM_WORD;}
 void gencore(int t,iparm &ip)
 {}
} STOP;



struct trapv:insttype
{
 trapv():insttype("0100_111001_110110","trapv",0)  {}
 void gencore(int t,iparm &ip)
 {
  OUT("test dword ptr [esp],IF_OVER");
  OUT("jz @@1");

  OUT("g68int V_TRAPV");

  OUT("@@1:");
 }
} TRAPV;

//-------------------------

struct jmp:insttype
{
 jmp():insttype("0100_111011_aaaaaa","jmp",(iAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR) )
  {naturalize=0;}
 void gencore(int t,iparm &ip)
 {
  OUT("mov esi,edx ; get pc");
  OUT("naturalizeESI");
 }
} JMP;


//-------------------------

struct jsr:insttype
{
 jsr():insttype("0100_111010_aaaaaa","jsr",(iAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR) )
  {naturalize=0;}
 void gencore(int t,iparm &ip)
 {
  OUT("denaturalizeESI");
  OUT("mov eax,edx ;save pc ");
  pushd("esi");
  OUT("mov esi,eax ; get pc");
  OUT("naturalizeESI");
 }
} JSR;



//-------------------------

struct rts:insttype
{
 rts():insttype("0100_111001_110101","rts",0)  {}
 void gencore(int t,iparm &ip)
 {
  popd("esi");
  OUT("naturalizeESI");
 }
} RTS;



//-------------------------

struct rtr:insttype
{
 rtr():insttype("0100_111001_110111","rtr",0)  {}
 void gencore(int t,iparm &ip)
 {
  OUT("getstackptr edx");
  OUT("add [edi.A7],6 ; add stackptr");

  //get flags
  OUT("add esp,4");
  OUT("mov al,[edx+1]");
  OUT("mov [edi.CCR],al");
  OUT("pushmflags");

  //get PC
  OUT("mov esi,[edx+2]");
  OUT("bswap esi");
  OUT("naturalizeESI");
 }
} RTR;

//--------------------------


struct rte:insttype
{
 rte():insttype("0100_111001_110011","rte",0)  {}
 void gencore(int t,iparm &ip)
 {
  OUT("getstackptr edx");
  OUT("add [edi.A7],6 ; add stackptr");

  //get flags
  OUT("add esp,4");
  OUT("mov al,[edx+1]");
  OUT("mov [edi.CCR],al");
  OUT("mov al,[edx]");
  OUT("mov [edi.SR],al");
  OUT("pushmflags");

  //get PC
  OUT("mov esi,[edx+2]");
  OUT("bswap esi");
  OUT("naturalizeESI");
 }
} RTE;




//------------------------

struct bcc:insttype
{
 bcc(char *f):insttype(f,"bcc",0 )
 {
  immtype=isx('l') ? IMM_NONE : IMM_WORD;
 }
 void gencore(int t,iparm &ip)
 {
  char s[32];
  sprintf(s,"BRANCH%d",isx('l') ? 8 : 16);

  switch (ip.c)
  {
   case 0: //bra
  printf("%s:\n",s);
  if (immtype==IMM_WORD)
  {
   OUT("movsx eax,bx");
   OUT("lea esi,[esi+eax-2]");
  } else
  {
   OUT("movsx eax,bh");
   OUT("add esi,eax ");
  }
  break;

  case 1: //bsr
  //push
  OUT("denaturalizeESI");
  OUT("mov eax,esi");
  pushd("eax");
  OUT("naturalizeESI");

  printf("\tjmp %s\n",s);
  break;

 case C_HI: OUT("popf\n\tpushf"); printf("\tja %s\n",s); break;
 case C_LS: OUT("popf\n\tpushf"); printf("\tjna %s\n",s); break;
 case C_CC: OUT("popf\n\tpushf"); printf("\tjnc %s\n",s);break;
 case C_CS: OUT("popf\n\tpushf"); printf("\tjc %s\n",s);break;
 case C_NE: OUT("popf\n\tpushf"); printf("\tjne %s\n",s); break;
 case C_EQ: OUT("popf\n\tpushf"); printf("\tje %s\n",s); break;
 case C_VC: OUT("popf\n\tpushf"); printf("\tjno %s\n",s); break;
 case C_VS: OUT("popf\n\tpushf"); printf("\tjo %s\n",s); break;
 case C_PL: OUT("popf\n\tpushf"); printf("\tjns %s\n",s); break;
 case C_MI: OUT("popf\n\tpushf"); printf("\tjs %s\n",s); break;
 case C_GE: OUT("popf\n\tpushf"); printf("\tjge %s\n",s); break;
 case C_LT: OUT("popf\n\tpushf"); printf("\tjl %s\n",s); break;
 case C_GT: OUT("popf\n\tpushf"); printf("\tjg %s\n",s); break;
 case C_LE: OUT("popf\n\tpushf"); printf("\tjle %s\n",s); break;
 default: OUT("FUCK!!!");
 }; //end case

 } //end core
}
BCC("0110_ccccll_llllll"),
BCC16("0110_cccc00_000000")
;


//---------------------------------

struct scc:insttype
{
 scc():insttype("0101_cccc11_aaaaaa","scc",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32) ) {}
 void gencore(int t,iparm &ip)
 {
  ip.dsize=0;

  char s[32];
  sprintf(s,"DOSET%X",t);

  OUT("xor eax,eax"); //fail
  switch (ip.c)
  {
   case 0: //salways
  printf("\t%s:\n",s);
  OUT("dec eax");
  break;

  case C_F: //snever
    break;

 case C_HI: OUT("popf\n\tpushf"); printf("\tja %s\n",s); break;
 case C_LS: OUT("popf\n\tpushf"); printf("\tjna %s\n",s); break;
 case C_CC: OUT("popf\n\tpushf"); printf("\tjnc %s\n",s);break;
 case C_CS: OUT("popf\n\tpushf"); printf("\tjc %s\n",s);break;
 case C_NE: OUT("popf\n\tpushf"); printf("\tjne %s\n",s); break;
 case C_EQ: OUT("popf\n\tpushf"); printf("\tje %s\n",s); break;
 case C_VC: OUT("popf\n\tpushf"); printf("\tjno %s\n",s); break;
 case C_VS: OUT("popf\n\tpushf"); printf("\tjo %s\n",s); break;
 case C_PL: OUT("popf\n\tpushf"); printf("\tjns %s\n",s); break;
 case C_MI: OUT("popf\n\tpushf"); printf("\tjs %s\n",s); break;
 case C_GE: OUT("popf\n\tpushf"); printf("\tjge %s\n",s); break;
 case C_LT: OUT("popf\n\tpushf"); printf("\tjl %s\n",s); break;
 case C_GT: OUT("popf\n\tpushf"); printf("\tjg %s\n",s); break;
 case C_LE: OUT("popf\n\tpushf"); printf("\tjle %s\n",s); break;
 default: OUT("FUCK!!!");
 }; //end case

 genwrite(t,ip.dsize);

 } //end core
} SCC;


//--------------------------


struct link:insttype
{
 link():insttype("0100_111001_010nnn","link",0)
  {immtype=IMM_WORD;}
 void gencore(int t,iparm &ip)
 {
  OUT("mov edx,[edi.A7]"); //get stack pointer
  OUT("sub edx,4");     //decrement stack

  //add disp to stack pointer
  OUT("movsx ebx,bx");
  OUT("add ebx,edx");
  OUT("mov [edi.A7],ebx");

  OUT("mov ebx,[edi.A0+eax*4]"); //get addr reg
  OUT("mov [edi.A0+eax*4],edx"); //set addr reg

  //resolve stack addr
  OUT("and edx,0FFFFh");
  OUT("add edx,[edi.RAMBASE]");
  OUT("bswap ebx");
  OUT("mov [edx],ebx"); //push addr reg
 }
} LINK ;



struct unlink:insttype
{
 unlink():insttype("0100_111001_011nnn","unlink",0) {}
 void gencore(int t,iparm &ip)
 {
  OUT("mov edx,[edi.A0+eax*4]"); //get saved stack pointer
  OUT("add edx,4");
  OUT("mov [edi.A7],edx"); //restore sp
  OUT("sub edx,4");

  //resolve stack addr
  OUT("and edx,0FFFFh");
  OUT("add edx,[edi.RAMBASE]");
  OUT("mov ebx,[edx]"); //pop addr reg
  OUT("bswap ebx");
  OUT("mov [edi.A0+eax*4],ebx");
 }
} UNLINK ;


//---------------------------------------------

//move to usp
struct moveusp:insttype
{
 moveusp():insttype("0100_111001_100nnn","moveusp",0) {}
 void gencore(int t,iparm &ip)
 {
//  OUT("mov edx,[edi.A0+eax*4]");
//  OUT("mov [edi.A7],edx");
 }
} MOVEUSP ;

//move from usp
struct moveusp2:insttype
{
 moveusp2():insttype("0100_111001_101nnn","moveusp2",0) {}
 void gencore(int t,iparm &ip)
 {
  OUT("mov edx,[edi.A7]");
  OUT("mov [edi.A0+eax*4],edx");
 }
} MOVEUSP2 ;




//--------------------------------------------


struct dbcc:insttype
{
 dbcc():insttype("0101_cccc11_001nnn","dbcc",0 )
  {immtype=IMM_WORD;}
 void gencore(int t,iparm &ip)
 {
  switch (ip.c)
  {
   case 0: //bra
  OUT("DBRANCH:");
  case 1: //bsr
  getreg("edx");
  OUT("dec dx");
  setreg("edx");
  OUT("cmp dx,-1");
  OUT("je executeloop");

  //branch
  OUT("movsx eax,bx");
  OUT("lea esi,[esi+eax-2]");
    return;

 case C_HI: OUT("popf\n\tpushf"); OUT("jna DBRANCH");break;
 case C_LS: OUT("popf\n\tpushf"); OUT("ja DBRANCH"); break;
 case C_CC: OUT("popf\n\tpushf"); OUT("jc DBRANCH"); break;
 case C_CS: OUT("popf\n\tpushf"); OUT("jnc DBRANCH");break;
 case C_NE: OUT("popf\n\tpushf"); OUT("je DBRANCH"); break;
 case C_EQ: OUT("popf\n\tpushf"); OUT("jne DBRANCH"); break;
 case C_VC: OUT("popf\n\tpushf"); OUT("jo DBRANCH");break;
 case C_VS: OUT("popf\n\tpushf"); OUT("jno DBRANCH"); break;
 case C_PL: OUT("popf\n\tpushf"); OUT("js DBRANCH");break;
 case C_MI: OUT("popf\n\tpushf"); OUT("jns DBRANCH");break;
 case C_GE: OUT("popf\n\tpushf"); OUT("jl DBRANCH"); break;
 case C_LT: OUT("popf\n\tpushf"); OUT("jge DBRANCH");break;
 case C_GT: OUT("popf\n\tpushf"); OUT("jle DBRANCH"); break;
 case C_LE: OUT("popf\n\tpushf"); OUT("jg DBRANCH"); break;
 default: OUT("FUCK!!!");
 }; //end case

 } //end core
} DBCC;













































