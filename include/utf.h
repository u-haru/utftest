#ifndef UNI_H
#define UNI_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

int utf8_Encode(uint32_t code,char* dest){
  memset(dest, 0, 4);
  if(code < 0x80){
    *dest = (char)code;
    return 1;
  }
  if(code < 0x800){
    *dest++ = ((code>>6)&0x1F)|0xC0;
    *dest = (code&0x3F)|0x80;
    return 2;
  }
  if(code < 0x10000){
    *dest++ = ((code>>12)&0x0F)|0xE0;
    *dest++ = ((code>>6)&0x3F)|0x80;
    *dest = (code&0x3F)|0x80;
    return 3;
  }
  *dest++ = ((code>>18)&0x07)|0xF0;
  *dest++ = ((code>>12)&0x3F)|0x80;
  *dest++ = ((code>>6)&0x3F)|0x80;
  *dest = (code&0x3F)|0x80;
  return 4;
}

uint32_t utf8_Decode(char p[]){
  uint32_t code = p[0];
  if(code < 0xC0){//1
    return code;
  }
  if(code < 0xE0){//2
    code &=0x1F;
    code = (code<<6) | (uint32_t)(p[1]&0x3F);
    return code;
  }
  if(code < 0xF0){//3
    code &=0x0F;
    code = (code<<6) | (uint32_t)(p[1]&0x3F);
    code = (code<<6) | (uint32_t)(p[2]&0x3F);
    return code;
  }
  //4
  code&=0x07;
  code = (code<<6) | (uint32_t)(p[1]&0x3F);
  code = (code<<6) | (uint32_t)(p[2]&0x3F);
  code = (code<<6) | (uint32_t)(p[3]&0x3F);
  return code;
}

int utf16_Encode(uint32_t code,short* dest){
  memset(dest, 0, 4);
  if(code < 0x10000){
    *dest = (short)code;
    return 1;
  }
  *dest++ = (((code>>10)&0x3FF)-0x40)|0xD800;
  *dest = (code&0x3FF)|0xDC00;
  return 2;
}

uint32_t utf16_Decode(short p[]){
  uint32_t code = p[0];
  if((code&0xFC00) == 0xD800 ){//1
    code = (code & 0x3FF)+0x40;
    code = (code << 10) | (p[1]&0x3FF);
  }
  return code;
}

#endif