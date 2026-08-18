struct mul:insttype
{
 mul():insttype("1100_nnnc11_aaaaaa","mul",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR|aimm) )
  {}
 void gencore(int t,iparm &ip)
 {
  ip.dsize=1;
  genread(t,ip.dsize);

  skipf();
  printf("\t%s word ptr [edi.D0+%d*4]\n",!ip.c ? "mul" : "imul",ip.n);
  pushf();
  printf("\tmov word ptr [edi.D0+%d*4],ax\n",ip.n);
  printf("\tmov word ptr [edi.D0+%d*4+2],dx\n",ip.n);
 }
} MUL;



struct div:insttype
{
 div():insttype("1000_nnnc11_aaaaaa","div",(Dn|iAn|iAni|idAn|d16An|d8AnR|addr16|addr32|d16PC|d8PCR|aimm) )
  {}
 void gencore(int t,iparm &ip)
 {
  ip.dsize=1;
  genread(t,ip.dsize);
  OUT("mov ebx,eax"); //divisor
  OUT("test ax,ax");
  OUT("jz executeloop");

  //dividend
  printf("\tmov ax,word ptr [edi.D0+%d*4]\n",ip.n);
  printf("\tmov dx,word ptr [edi.D0+%d*4+2]\n",ip.n);

  skipf();
  printf("\t%s bx\n",!ip.c ? "div" : "idiv");
  pushf();
  printf("\tmov word ptr [edi.D0+%d*4],ax\n",ip.n);
  printf("\tmov word ptr [edi.D0+%d*4+2],dx\n",ip.n);
 }
} DIV;
