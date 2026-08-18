
struct moveq:insttype
{
 moveq():insttype("0111_nnn0cc_cccccc","moveq",0)
  {}
 void gencore(int t,iparm &ip)
 {
  OUT("movsx edx,bh ;get val");
  skipf();
  OUT("test bh,bh");
  pushf();
  printf("\tmov [edi.D0+%d*4],edx ;set reg\n",ip.n);
 }
} MOVEQ;


//------------------------------------

//move to ccr
struct movebccr:insttype
{
 movebccr():insttype("0100_010011_aaaaaa","movebccr",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR|aimm)) {}
 void gencore(int t,iparm &ip)
 {
  ip.dsize=0;
  genread(t,ip.dsize);
  skipf();
  printf("\tmov %s,al \n",ccrreg);
  OUT("pushmflags");
 }
} MOVEBCCR;



//move to sr
struct movewsr:insttype
{
 movewsr():insttype("0100_011011_aaaaaa","movewsr",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR|aimm)) {}
 void gencore(int t,iparm &ip)
 {
  ip.dsize=1;
  genread(t,ip.dsize);
  skipf();
  printf("\tmov %s,ax \n",srreg);
  OUT("pushmflags");
 }
} MOVEWSR;


//move from sr
struct movewsr2:insttype
{
 movewsr2():insttype("0100_000011_aaaaaa","movewsr2",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32)) {}
 void gencore(int t,iparm &ip)
 {
  ip.dsize=1;
  OUT("popmflags");
  OUT("pushmflags");
  printf("\tmov ax,%s\n",srreg);
  genwrite(t,ip.dsize);
 }
} MOVEWSR2;




//-----------------------------------

struct move:insttype
{
 move():insttype("00vv_dddddd_aaaaaa","move",(Dn|An|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR) )
  {}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize); //eax=read from source

  if (ip.dmode<0x10) //direct?
   {
    //set flags
    skipf();
    int num=ip.dmode&7;
    if (ip.dmode<8)
     printf("\tmov %s ptr [edi.D0+%d*4],%s\n",isize[ip.dsize],num,areg[ip.dsize]);
    else
    {
     if (ip.dsize==1)  OUT("cwde");
     printf("\tmov [edi.A0+%d*4],%s\n",num,areg[2]);
    }
    printf("\ttest %s,%s \n",areg[ip.dsize],areg[ip.dsize]);
    pushf();
    return;
   }

  //write
  OUT("push eax");
  if (ip.dmode<0x38)
      {
       int num=ip.dmode&7;
       if (num) printf("\tmov eax,%d\n",num); else OUT("xor eax,eax");
        if (ip.dsize==0 && (ip.dmode==0x1F || ip.dmode==0x27)) genlea(ip.dmode,ip.dsize);
          else  genlea(ip.dmode&(~7),ip.dsize);
      } else  genlea(ip.dmode,ip.dsize); //get dest address
  OUT("pop eax");

  //set flags
  skipf();
  printf("\ttest %s,%s \n",areg[ip.dsize],areg[ip.dsize]);
  pushf();

  OUT("naturalize edx,@@WRITEM,@@TRAP");

  OUT("@@WRITEM:");
  genwrite(DIRECTM,ip.dsize);
  OUT("executenext");

  OUT("@@TRAP:");
  genwrite(TRAP,ip.dsize);
 }
} MOVE;



//----------------------------

//source is immediate
struct moveimm:insttype
{
 moveimm():insttype("00vv_dddddd_111100","moveimm",0)
  {immtype=IMM_DSIZE;}
 void gencore(int t,iparm &ip)
 {
  genread(t,ip.dsize); //eax=read from source

  //write
  if (ip.dmode<0x38)
      {
       int num=ip.dmode&7;
       if (num) printf("\tmov eax,%d\n",num); else OUT("xor eax,eax");
        if (ip.dsize==0 && (ip.dmode==0x1F || ip.dmode==0x27)) genlea(ip.dmode,ip.dsize);
         else  genlea(ip.dmode&(~7),ip.dsize);
      } else  genlea(ip.dmode,ip.dsize); //get dest address

  //set flags
  skipf();
  printf("\tmov eax,%s \n",immreg[2]);
  printf("\ttest %s,%s \n",immreg[ip.dsize],immreg[ip.dsize]);
  pushf();

  if (ip.dmode>=0x10) //write to mem space
   {
    OUT("naturalize edx,@@WRITEM,@@TRAP");

    OUT("@@WRITEM:");
    genwrite(DIRECTM,ip.dsize);
    OUT("executenext");

    OUT("@@TRAP:");
    genwrite(TRAP,ip.dsize);
   } else
   {
    if (ip.dmode>=0x8 && ip.dsize==1)
     {
      OUT("cwde");
      genwrite(DIRECTI,2);
     } else  genwrite(DIRECTI,ip.dsize);
   }
 }
} MOVEIMM;


//-----------------------------------------

//reglist-><ea>
struct movem:insttype
{
 movem():insttype("0100_10001y_aaaaaa","movem",(iAn|d16An|d8AnR|addr16|addr32))
   {immtype=IMM_WORD;}
 void gencore(int t,iparm &ip)
 {
  //edx=dest address bx=reg bits
  if (t>0) printf("\tMOVEM_WRITE_MACRO %d,%d\n",ip.dsize-1,t-1);
   else
    {
     OUT("sub esi,2");
     OUT("jmp badopcode");
    }
 }
} MOVEM;



//reglist pre decriment-><ea>
struct movempd:insttype
{
 movempd():insttype("0100_10001y_100nnn","movempd",0)
   {immtype=IMM_WORD;}
 void gencore(int t,iparm &ip)
 {
  OUT("mov edx,[edi.A0+eax*4]"); //get An

  OUT("naturalize edx,@@WRITEM,@@TRAP");

  OUT("@@TRAP:");
//  printf("\tMOVEM_WRITEREVERSE_MACRO %d,1\n",ip.dsize-1);
//  OUT("add [edi.A0+eax*4],ebx"); //add post increment
//  OUT("executenext");
  OUT("sub esi,2");
  OUT("jmp badopcode");

  OUT("@@WRITEM:");
  printf("\tMOVEM_WRITEREVERSE_MACRO %d,0\n",ip.dsize-1);
  OUT("sub [edi.A0+eax*4],ebx"); //sub predecrement
 }
} MOVEMPD;




//-----------------------------------------



//<ea>->reglist
struct movem2:insttype
{
 movem2():insttype("0100_11001y_aaaaaa","movem2",(iAn|d16An|d8AnR|addr16|addr32))
   {immtype=IMM_WORD;}
 void gencore(int t,iparm &ip)
 {
  //edx=dest address bx=reg bits
  if (t>0) printf("\tMOVEM_READ_MACRO %d,%d\n",ip.dsize-1,t-1);
   else
    {
     OUT("sub esi,2");
     OUT("jmp badopcode");
    }
 }
} MOVEM2;


//-------------------------------------------


//<ea> post inc ->reglist
struct movem2pi:insttype
{
 movem2pi():insttype("0100_11001y_011nnn","movem2pi",0)
   {immtype=IMM_WORD;}
 void gencore(int t,iparm &ip)
 {
  OUT("mov edx,[edi.A0+eax*4]"); //get An

  OUT("naturalize edx,@@WRITEM,@@TRAP");

  OUT("@@TRAP:");
//  printf("\tMOVEM_READ_MACRO %d,1\n",ip.dsize-1);
//  OUT("add [edi.A0+eax*4],ebx"); //add post increment
//  OUT("executenext");
  OUT("sub esi,2");
  OUT("jmp badopcode");

  OUT("@@WRITEM:");
  printf("\tMOVEM_READ_MACRO %d,0\n",ip.dsize-1);
  OUT("add [edi.A0+eax*4],ebx"); //add post increment
 }
} MOVEM2PI;














