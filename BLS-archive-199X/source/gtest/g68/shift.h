



//shift instruction
struct shift:insttype
{
 int dogetx;
 int dosetx;
 int type;
 char *intelinst;

 shift(char *f,char *n,int a,int t,char *ii,int dgx,int dsx):
  insttype(f,n,a),type(t),intelinst(ii),dogetx(dgx),dosetx(dsx) {}
 void gencore(int t,iparm &ip)
 {
  switch (type)
  {
   case 1: //get register
   printf("\tmov cl,byte ptr [edi.D0+%d*4]\n",ip.c);
   OUT("and cl,63d");
   break;
   case 2: //get data
   genread(t,ip.dsize);
   break;
  }

  skipf();
  if (dogetx) getxflag();
  switch (type)
  {
   //immediate
   case 0: printf("\t%s %s ptr [edi.D0+eax*4],%d\n",intelinst,isize[ip.dsize],ip.c ? ip.c : 8); break;
   //other register
   case 1: printf("\t%s %s ptr [edi.D0+eax*4],cl\n",intelinst,isize[ip.dsize]); break;
   //memory
   case 2: printf("\t%s %s,1\n",intelinst,areg[ip.dsize]); break;
  }

  if (dosetx) pushfx(); else pushf();

  switch (type)
  {
   case 2: //set data
   genwrite(t,ip.dsize);
   break;
  }
 }
}

 ASRDI("1110_ccc0zz_000nnn","asrdi",0, 0,"sar",0,1),
 ASLDI("1110_ccc1zz_000nnn","asldi",0, 0,"sal",0,1),
 LSRDI("1110_ccc0zz_001nnn","lsrdi",0, 0,"shr",0,1),
 LSLDI("1110_ccc1zz_001nnn","lsldi",0, 0,"shl",0,1),
 ROXRDI("1110_ccc0zz_010nnn","roxrdi",0, 0,"rcr",1,1),
 ROXLDI("1110_ccc1zz_010nnn","roxldi",0, 0,"rcl",1,1),
 RORDI("1110_ccc0zz_011nnn","rordi",0, 0,"ror",0,0),
 ROLDI("1110_ccc1zz_011nnn","roldi",0, 0,"rol",0,0),

 ASRD("1110_ccc0zz_100nnn","asrd",0, 1,"sar",0,1),
 ASLD("1110_ccc1zz_100nnn","asld",0, 1,"sal",0,1),
 LSRD("1110_ccc0zz_101nnn","lsrd",0, 1,"shr",0,1),
 LSLD("1110_ccc1zz_101nnn","lsld",0, 1,"shl",0,1),
 ROXRD("1110_ccc0zz_110nnn","roxrd",0, 1,"rcr",1,1),
 ROXLD("1110_ccc1zz_110nnn","roxld",0, 1,"rcl",1,1),
 RORD("1110_ccc0zz_111nnn","rord",0, 1,"ror",0,0),
 ROLD("1110_ccc1zz_111nnn","rold",0, 1,"rol",0,0);



struct shifta:shift
{
 shifta(char *f,char *n,char *ii, int dgx,int dsx):
  shift(f,n,(iAn|iAni|idAn|d16An|d8AnR|addr16|addr32), 2,ii,dgx,dsx) {};

 void gencore(int t,iparm &ip)
 {
  ip.dsize=1; //word
  shift::gencore(t,ip);
 }
}

  ASRDA("1110_000011_aaaaaa","asrda", "sar",0,1),
  ASLDA("1110_000111_aaaaaa","aslda", "sal",0,1),
  LSRDA("1110_001011_aaaaaa","lsrda", "shr",0,1),
  LSLDA("1110_001111_aaaaaa","lslda", "shl",0,1),
 ROXRDA("1110_010011_aaaaaa","roxrda", "rcr",1,1),
 ROXLDA("1110_010111_aaaaaa","roxlda", "rcl",1,1),
  RORDA("1110_011011_aaaaaa","rorda", "ror",0,0),
  ROLDA("1110_011111_aaaaaa","rolda", "rol",0,0)
;




























