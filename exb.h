#ifndef __EXB_H__
#define __EXB_H__
#include "base.h"
char* ex_coding(char* text,int len,char* coding,char* tocoding);
int UTF8ToUni( char *utf8str, char *unistr, int size);
int UniToUTF8(char *unistr, char *utf8str, int size);
char *gbToUn(char *text);
char *unToGb(char *text);
#endif
