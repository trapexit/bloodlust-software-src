#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "file.h"
#include "compress.h"



void main()
{
 char s[80];



//compress file
{
 FILEIO in,out;
 out.create("file.cmp");

 {
  //buffer input and output
  BUFFERI bin(&in,64000);
  BUFFERO bout(&out,64000);

  //create compression class...
  COMPRESS c(&bout);

  //compress to file.out
  in.open("file.in");
  c.start(); //begin segment
  bin>>c;
  c.stop();  //terminate segment
  printf("compressed %d bytes\n",in.getsize());
  in.close();

  //compress to file.out
  in.open("file2.in");
  c.start(); //begin segment
  bin>>c;
  c.stop();  //terminate segment
  printf("compressed %d bytes\n",in.getsize());
  in.close();

 }
// printf("compressed %d bytes to %d bytes\n",in.getsize(),out.getsize());
}


//decompress file
{
 FILEIO in;
 in.open("file.cmp");

 //buffer input and output
 BUFFERI bin(&in,4096);

 //create decompression class...
 DECOMPRESS d(&bin);


 //decompress file.cmp to file.out
 {
  FILEIO out;
  out.create("file.out");
  BUFFERO bout(&out,4096);
  d>>bout;
 }

 //decompress file.cmp to file.out
 {
  FILEIO out;
  out.create("file2.out");
  BUFFERO bout(&out,4096);
  d>>bout;
 }

}

// printf("f=%s\n",f.printinfo(s));
// printf("m=%s\n",m.printinfo(s));
// printf("c=%s\n",c.printinfo(s));
}    
