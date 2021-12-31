#include <stdio.h>
#include "utf.h"

const char zwnbs[] = {0xFF,0xFE};
const char bom[] = {0xEF,0xBB,0xBF};

int main(){
  uint32_t codepoints[4] = {0x61,0x3B1,0x3042,0x1F600};
  int length;
  char buf[4];

  FILE *fp_le = fopen("./utf16le.txt","wb");
  FILE *fp_be = fopen("./utf16be.txt","wb");
  FILE *fp_8 = fopen("./utf8.txt","wb");

  // write bom
  fwrite(&zwnbs[0],2,1,fp_le);
  
  fwrite(&zwnbs[1],1,1,fp_be);
  fwrite(&zwnbs[0],1,1,fp_be);

  fwrite(bom,3,1,fp_8);
  // write bom

  for(uint32_t i = 0;i<4;i++){
    length = utf16_Encode(codepoints[i],(short*)buf);
    // write as utf-16 LE(or BE)
    fwrite(&buf[0],2,1,fp_le);
    if(length==2){
      fwrite(&buf[2],2,1,fp_le);
    }

    // write as utf-16 BE(or LE)
    fwrite(&buf[1],1,1,fp_be);
    fwrite(&buf[0],1,1,fp_be);
    if(length==2){
      fwrite(&buf[3],1,1,fp_be);
      fwrite(&buf[2],1,1,fp_be);
    }

    length = utf8_Encode(codepoints[i],buf);
    // write as utf-8
    fwrite(buf,1,length,fp_8);
  }
  fclose(fp_le);
  fclose(fp_be);
  fclose(fp_8);
  return 0;
}