
struct bitinst:insttype
{
 char *intelinst;
 char *jumpinst;
 int dochange;
 bitinst(char *f,char *n,int a,char *ii,char *ji,int c):insttype(f,n,a)
  {
   intelinst=ii;
   jumpinst=ji;
   dochange=c;
   immtype=isx('n') ? IMM_NONE : IMM_BYTE;
  }

 void gencore(int t,iparm &ip)
 {
  if (!t) ip.dsize=2; else ip.dsize=0;
  genread(t,ip.dsize);

  if (ip.n!=-1)
   printf("\tmov ebx,[edi.D0+%d*4]\n",ip.n);

  OUT("mov cl,[esp]");
  printf("\tand ebx,%s\n",ip.dsize==2 ? "31" : "7");
  OUT("and cl,NOT IF_ZERO");

//  OUT("and byte ptr [esp],NOT IF_ZERO ;clear zero");
  printf("\t%s eax,ebx ;test\n",intelinst,areg[ip.dsize]);
  printf("\t%s  @@1\n",jumpinst);
//  OUT("or  byte ptr [esp],IF_ZERO");
  OUT("or cl,IF_ZERO");
  OUT("@@1:");
  OUT("mov [esp],cl");

  if (dochange) genwrite(t,ip.dsize);
 }

}
 BTSTI("0000_100000_aaaaaa","btsti",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR),"bt","jc",0),
 BCHGI("0000_100001_aaaaaa","bchgi",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32),"btc","jnc",1),
 BCLRI("0000_100010_aaaaaa","bclri",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32),"btr","jc",1),
 BSETI("0000_100011_aaaaaa","bseti",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32),"bts","jc",1	)
 ,

 BTST("0000_nnn100_aaaaaa","btst",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR),"bt","jc",0),
 BCHG("0000_nnn101_aaaaaa","bchg",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32),"btc","jnc",1),
 BCLR("0000_nnn110_aaaaaa","bclr",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32),"btr","jc",1),
 BSET("0000_nnn111_aaaaaa","bset",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32),"bts","jc",1	)

 ;


