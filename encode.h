#ifndef _ENCODE_H
#define _ENCODE_H

#include "base.h"
// #include "mr_types.h"

//gb转unicode(大端)
 unsigned char *GBCodeToUnicode(const unsigned char *gbCode);
//GB2312 -> UCS2LE
int gbToUCS2(unsigned char *gbCode, unsigned char *unicode);
int gbToUCS2len(const unsigned char *gbCode, unsigned char *unicode,int len);
int UCS2ToGBString(const uint16 *uniStr, uint8 *gbBuf, int gbMaxLen);
int UTF8ToUCS2String(const uint8 *utf8Str, uint16 *ucs2Buf, int ucs2MaxLen);
int UTF8ToGBString(const uint8 *utf8Str, uint8 *gbBuf, int gbMaxLen);
int GBToUTF8String(const uint8 *gbStr, uint8 *utf8Buf, int utf8MaxLen);
//unicode unicode小端互转
char *UCS2ByteRev(char * str);
//unicode小端转utf
int UCS2ToUTF8(const uint8 *unicode, uint8 *utf8, int size);
int UCS2_strlen(const char * txt);





#endif
