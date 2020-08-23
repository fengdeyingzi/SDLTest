#include <stdio.h>
#include <stdint.h>

#define TOAST_SHORT_TIME 2000
#define TOAST_LONG_TIME 3000
extern uint32_t toast_end_time;
void *readFileFromAssets(char *filename, int *len);
void toast(char *text, int type);
