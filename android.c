#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "graphics.h"
#include <fcntl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
// #include <windows.h>

extern SDL_Surface *surface_ui;
uint32_t toast_end_time;
int timer_ui;

/*
 * Return the pixel value at (x, y) 获取像素
 * NOTE: The surface must be locked before calling this!
 */
static Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0; /* shouldn't happen, but avoids warnings */
    }
}

/*
 * Set the pixel at (x, y) to the given value 设置像素
 * NOTE: The surface must be locked before calling this!
 */
static void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    switch (surface->format->BytesPerPixel)
    {
    case 1: // Assuming 8-bpp
    {
        Uint8 *bufp;
        bufp = (Uint8 *)surface->pixels + y * surface->pitch + x;
        *bufp = pixel;
    }
    break;
    case 2: // Probably 15-bpp or 16-bpp
    {
        Uint16 *bufp;
        bufp = (Uint16 *)surface->pixels + y * surface->pitch / 2 + x;
        *bufp = pixel;
    }
    break;
    case 3: // Slow 24-bpp mode, usually not used
    {
        Uint8 *bufp;
        bufp = (Uint8 *)surface->pixels + y * surface->pitch + x * 3;
        if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
            bufp[0] = pixel;
            bufp[1] = pixel >> 8;
            bufp[2] = pixel >> 16;
        }
        else
        {
            bufp[2] = pixel;
            bufp[1] = pixel >> 8;
            bufp[0] = pixel >> 16;
        }
    }
    break;
    case 4: // Probably 32-bpp
    {
        Uint32 *bufp;
        bufp = (Uint32 *)surface->pixels + y * surface->pitch / 4 + x;
        *bufp = pixel;
    }
    break;
    }
}


static void UI_drawPoint(int x,int y,unsigned int color){
    // SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
    // SDL_RenderDrawPoint(renderer,x,y);
    // uint32 ncolor = (color<<8) | (color>>24);
    
    if(x>=0&&y>=0&&x<SCRW&&y<SCRH){
        uint32 scolor = get_pixel(surface_ui,x,y);
        uint32 gcolor = gl_getColor(scolor>>8,color);
        put_pixel(surface_ui,x,y,gcolor<<8|0xff);
    }
    
}

//画矩形
static void UI_drawRect(int x, int y, int w, int h, unsigned int color)
{
    // SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
    // SDL_Rect rect = {x, y, w, h};
    // SDL_RenderFillRect(renderer, &rect); //
    for(int ix=x;ix<x+w;ix++){
        for(int iy=y;iy<y+h;iy++){
            if(ix>=0&&iy>=0&& ix<SCRW&& iy<SCRH)
            UI_drawPoint(ix,iy,color);
        }
    }
}

void *readFileFromAssets(char *filename, int *len){
    char temp[300];
    sprintf(temp,"assets\\%s",filename);
    *len = getlen(temp);
    void *buf = malloc(*len);
    int f = open(temp, O_RDONLY, 0777);
    read(f,buf,*len);
    close(f);
    return buf;
}

void UI_logoc(int data){
    //清除
    for(int ix=0;ix<SCRW;ix++){
        for(int iy=0;iy<SCRH;iy++){
            put_pixel(surface_ui,ix,iy, 0);
        }
    }
    ref(0,0,0,0);
}

void toast(char *text, int type){
    TTF_Font *font = TTF_OpenFont("assets/font.ttf", 20);
    if (!font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
    int x,y;
    int w,h;
    TTF_SizeUTF8(font, text, &w,&h);
    toast_end_time = SDL_GetTicks()+2000;
    if(type==1){
        toast_end_time += 1000;
    }
    
    timer_ui = timercreate();
    timerstart(timer_ui, toast_end_time-SDL_GetTicks(), 1, UI_logoc,0);

    x = (SCRW-w)/2;
    y = (SCRH*2/3)-h/2;
    SDL_Color scolor = {0xff, 0xff, 0xff, 0xff}; //颜色
    SDL_Surface *text_surface;
    UI_drawRect(x-16,y-16,w+32,h+32,0xff202020);

    text_surface = TTF_RenderUTF8_Solid(font, text, scolor);
    if (text_surface != NULL)
    {
       
        // SDL_FreeSurface(text_surface);
        SDL_Rect srcrect = {0, 0, text_surface->w, text_surface->h};
        SDL_Rect dstrect = {x, y, text_surface->w, text_surface->h};
        //往渲染器上copy字体
        //<-----------------第二步：渲染文字即可----------------------------->
        // SDL_RenderCopy(renderer, pTexture, &srcrect, &dstrect);
        // SDL_DestroyTexture(pTexture);
        printf("drawText 1\n");
        SDL_BlitSurface(text_surface, &srcrect, surface_ui, &dstrect);//将文字复制到屏幕的surface上面
        SDL_FreeSurface(text_surface);
        printf("drawText ok\n");
        return;
    }
    else
    {
        // report error
        printf("drawText error\n");
        return;
    }
    // MessageBox(NULL,(LPCSTR)"hello world",(LPCSTR)"Title", MB_OK);
    ref(0,0,SCRW,SCRH);
}
