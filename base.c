#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "base.h"
#include "graphics.h"
#include <fcntl.h>
#include "utils.h"
#include <sys/stat.h>
// #include <sys/time.h>
// #undef BITMAPINFO
#include <windows.h>

#include <time.h> //时间
extern SDL_Surface *surface_window;
extern SDL_Surface *surface_cache;
extern SDL_Window *window;
int SCRH = 480;
int SCRW = 720;


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


//初始化
void base_init(){
    
}



//请屏
void cls(int r,int g,int b){
    uint32 color = ((r<<24)|(g<<16)|(b<<8)|255);
    for(int ix=0;ix<surface_cache->w;ix++){
        for(int iy=0;iy<surface_cache->h;iy++){
            put_pixel(surface_cache, ix,iy,color);
        }
    }
    // SDL_RenderClear(renderer);
}

void LOG_I(char *text){
    int f = open("C/print.txt", O_RDONLY|O_WRONLY|O_CREAT, 0777);
    if(f>=0){
        lseek(f, 0, SEEK_END);
    write(f,text,strlen(text));
    close(f);
    }
    
}

//刷新屏幕
void ref(int x,int y,int w,int h){
    SDL_Rect srcRect = {x, y, w, h};
    SDL_Rect dstRect = {x, y, w, h};
    //Blit操作
    SDL_BlitSurface(surface_cache, &srcRect, surface_window, &dstRect);
    SDL_UpdateWindowSurface(window);
}

void capp_exit(){
    SDL_FreeSurface(surface_window);
    SDL_FreeSurface(surface_cache);
    SDL_DestroyWindow(window);
}

int getlen(const char* filename)
{
	char fullpathname[255] = { 0 };
	struct stat s1;
	int ret;

	ret = stat(filename, &s1);

	if (ret != 0)
		return -1;

	return s1.st_size;
}

/*
un编码 大端小端
获取长度
*/
int wstrlen(const char * txt)
{
	int i=0;
    if(txt==NULL) return 0;
	while(txt[i] || txt[i+1]) i += 2;

	return i;
}

unsigned int getuptime(){
	// time_t tt = time(NULL);//这句返回的只是一个时间cuo
	// struct tm* t = localtime(&tt);
	// // printf("\n%d-%02d-%02d %02d:%02d:%02d\n",
	// // 	t->tm_year + 1900,
	// // 	t->tm_mon + 1,
	// // 	t->tm_mday,
	// // 	t->tm_hour,
	// // 	t->tm_min,
	// // 	t->tm_sec);
    // return (t->tm_year + 1900)*12*
#ifdef WIN32
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year = wtm.wYear - 1900;
    tm.tm_mon = wtm.wMonth - 1;
    tm.tm_mday = wtm.wDay;
    tm.tm_hour = wtm.wHour;
    tm.tm_min = wtm.wMinute;
    tm.tm_sec = wtm.wSecond;
    tm.tm_isdst = -1;
    clock = mktime(&tm);
    unsigned int timestamp = clock * 1000 + wtm.wMilliseconds;  //获取豪秒数
    return timestamp;
#else
//     // struct timeval tv;
//     // gettimeofday(&tv, NULL);
//     // timestamp = tv.tv_sec * 1000000 + tv.tv_usec;
#endif
    // return SDL_GetPerformanceCounter();
}

int timercreate(){
    _TIMER *timer = (_TIMER *)malloc(sizeof(_TIMER));
    memset(timer,0, sizeof(_TIMER));
    return (int)timer;
}

Uint32 capp_timerRun(Uint32 interval, void *param)
{
    printf("this is timer\n");
    _TIMER *timer = (_TIMER *)param;

    if(!timer->loop){
        timer->isStop = 1;
SDL_RemoveTimer(timer->timerId);
    }
    // timer->timerCB(timer->data);
    SDL_Event event;// = (SDL_Event*)malloc(sizeof(SDL_Event));
    SDL_UserEvent userevent;
    userevent.timestamp = SDL_GetTicks();
    userevent.code = SDL_TIMER;
    userevent.type = SDL_USEREVENT;
    userevent.data1 = timer;
    userevent.data2 = timer->timerCB;
    event.type = SDL_USEREVENT;
    event.user = userevent;
    SDL_PushEvent(&event);
    return interval;
}

int timerstart(int t, int time, int data, mrc_timerCB timerCB, int loop){
    _TIMER *timer = (_TIMER *)t;
    timer->data = data;
    timer->loop = loop;
    timer->isrun = 1;
    timer->timerCB = timerCB;
    timer->timerId = SDL_AddTimer(time, capp_timerRun ,timer);
    return 0;
}

int timerstop(int t){
    _TIMER *timer = (_TIMER *)t;
    if(!timer->isStop){
        timer->isStop = 1;
        SDL_RemoveTimer(timer->timerId); 
    }
    
}

int timerdel(int t){
    _TIMER *timer = (_TIMER *)t;
    if(!timer->isStop)
    SDL_RemoveTimer(timer->timerId);
    free(timer);
}

int textwh(char *pcText, int is_unicode, int font, int* w, int* h){
    getTextWH(pcText, is_unicode, font, w,h);
    return 0;
}

int dtext(char *pcText, int x,int y, int r,int g, int b, int is_unicode, int font){
    drawText(pcText, x, y,(0xff000000|(r<<16)|(g<<8)|b),is_unicode, font);
    return 0;
}

int dtextex(char *pcText, int x,int y, rectst *rect, colorst *color, int is_unicode, int font){
    drawTextEx(pcText, x, y,rect, (0xff000000|(color->r<<16)|(color->g<<8)|color->b), is_unicode, font);
    return 0;
}

int dtextright(char *pcText, int x,int y, rectst *rect, colorst *color, int is_unicode, int font){
    int w=0,h=0;
    getTextWH(pcText, is_unicode, font, &w,&h);
    drawText(pcText, x-w, y, (0xff000000|(color->r<<16)|(color->g<<8)|color->b), is_unicode, font);
    return 0;
}

void sleep(int ms){
    // Sleep(ms);
    SDL_Delay(ms);
}

int shake(int ms){
    return 0;
}



void drect(int x,int y,int w,int h,int r,int g,int b){
    uint32 color = ((r<<24)|(g<<16)|(b<<8)|255);
    for(int ix=x;ix<x+w;ix++){
        for(int iy=y;iy<y+h;iy++){
            if(ix>=0&& iy>=0 && ix<SCRW && iy<SCRH)
            put_pixel(surface_cache, ix,iy,color);
        }
    }
}

void dline(int x1,int y1,int x2,int y2,int r, int g,int b){
     	int x, y, dx, dy, c1, c2, err, swap = 0;
uint32 color = ((r<<24)|(g<<16)|(b<<8)|255);
#ifdef MR_ANYKA_MOD
	uint32 nativecolor;
#else
	uint16 nativecolor;
#endif
	/*
	nativecolor = (r/8)<<11;
	nativecolor |=(g/4)<<5;
	nativecolor |=(b/8);     
	*/
	nativecolor = MAKERGB(r, g, b);

	/*   
	if (x1 < 0 || x1 >= MR_SCREEN_W || x2 < 0 || x2 >= MR_SCREEN_W ||
	y1 < 0 || y1 >= MR_SCREEN_H || y2 < 0 || y2 >= MR_SCREEN_H)
	return;
	*/

	dx = x2 - x1; dy = y2 - y1;
	if (((dx < 0) ? -dx : dx) < ((dy < 0) ? -dy : dy))
	{
		swap = 1;                       /* take the long way        */
		x = x1; x1 = y1; y1 = x;
		x = x2; x2 = y2; y2 = x;
	}
	if (x1 > x2)
	{
		x = x1; x1 = x2; x2 = x;        /* always move to the right */
		y = y1; y1 = y2; y2 = y;
	}

	dx = x2 - x1; dy = y2 - y1;
	c1 = dy * 2; dy = 1;
	if (c1 < 0)
	{
		c1 = -c1;
		dy = -1;
	}
	err = c1 - dx; c2 = err - dx;
	x = x1; y = y1;
	while (x <= x2)
	{
		dpointex((int16)(swap?y:x),(int16)(swap?x:y),r,g,b);
		x++;
		if (err < 0)
			err += c1;
		else
		{
			y += dy;
			err += c2;
		}
	}
}

void dpointex(int x,int y,int r,int g,int b){
    uint32 color = ((r<<24)|(g<<16)|(b<<8)|255);
    if(x>=0&& y>=0 && x<SCRW&&y<SCRH)
    put_pixel(surface_cache, x,y,color);
}

void getscrsize(int *w,int *h){
    *w = SCRW;
    *h = SCRH;
}

void setscrsize(int w,int h){
    SCRW = w;
    SCRH = h;
    SDL_SetWindowSize(window, SCRW,SCRH);
    int rmask = 0xFF000000; int gmask = 0x00FF0000; int bmask = 0x0000FF00; int amask = 0x000000FF;	// RGBA8888模式
    SDL_FreeSurface(surface_cache);
    surface_cache = SDL_CreateRGBSurface(SDL_PREALLOC, w, h , 32, rmask, gmask, bmask, amask);
    SDL_SetSurfaceBlendMode(surface_cache,SDL_BLENDMODE_BLEND);


    surface_window = SDL_GetWindowSurface(window);
    // surface_cache = SDL_CreateRGBSurface(SDL_PREALLOC, surface_window->w, surface_window->h , 32, rmask, gmask, bmask, amask);
    SDL_UpdateWindowSurface(window);
}

int32 effsetcon(int16 x, int16 y, int16 w, int16 h, int16 perr, int16 perg, int16 perb)
{
	int16 x_d, y_d;
	int16 x_min, y_min;
	int16 x_max, y_max;
	uint32 pixel_new;
	uint32 pixel;
	uint32 p;



	y_max = MIN(h + y, SCRH);
	x_max = MIN(w + x, SCRW);
	y_min = MAX(y, 0);
	x_min = MAX(x, 0);

	for (y_d = y_min; y_d < y_max; y_d++)
	{
		
		for (x_d = x_min; x_d < x_max; x_d++)
		{
            p = get_pixel(surface_cache, x_d,y_d);
			pixel = p;
			pixel_new  = (perr * ((pixel & 0xff000000) >> 8)) & 0xff000000;
			pixel_new |= (perg * ((pixel & 0xff0000) >> 8)) & 0xff0000;
			pixel_new |= (perb * ((pixel & 0xff00) >> 8)) & 0xff00;
            pixel_new |= 0xff;
			p = pixel_new;
			put_pixel(surface_cache, x_d, y_d, pixel_new);
		}
	}

    return 0;
}

int  dlgcreate(char *title,char *text,int type){
    return 0;
}

int dlgdel(int dialog){
    return 0;
}

void sand(unsigned int num){

}

void img(char *filename, int x,int y){
    int bitmap = readBitmap(filename);
    if(bitmap){
        drawBitmap(bitmap,x,y);
        bitmapFree(bitmap);
    }
}
