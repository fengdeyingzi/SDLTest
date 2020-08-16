/*
SDL 实现手机C的graphics.h
*/
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "base.h"


enum{
    _JPG = 0,
    _PNG = 1,
    _BMP16 = 2
};

typedef struct{
    unsigned int width;
    unsigned int height;
    unsigned int stride;
    unsigned int format;
    unsigned int flags;
    void *ptr;
} _BITMAPINFO;


int createBitmap(int w, int h);
void graphics_init(SDL_Window *win, SDL_Renderer *render);
void graphics_free();
void setDrawRenderer(SDL_Renderer *render);
void drawText(char *text, int x, int y,unsigned int color, int is_unicode, int font_type);
void drawTextEx(char *text, int x,int y, rectst *rect, unsigned int color, int is_unicode, int font_type);

void getTextWH(char *text,int is_unicode, int font_type, int *w, int *h);
int readBitmap(char *filename);
int readBitmapFromAssets(char *filename);
void drawBitmap(int bmp, int x,int y);

void drawBitmapFlip(int bmp, int x,int y, int w,int h, int tx,int ty);
void drawBitmapEx(int bmp, int x,int y,int w,int h, int tx,int ty,int tw,int th);
void bitmapFree(int bmp);

void drawPoint(int x,int y,unsigned int color);
//画矩形
void drawRect(int x, int y, int w, int h, unsigned int color);

void drawCircle(int x,int y,int radius, unsigned int color);
#define drawCir drawCircle
void drawLine(int x1,int y1,int x2,int y2,unsigned int color);

#endif
