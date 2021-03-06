#include <stdio.h>
#include <SDL2/SDL.h>
#include "base.h"
#include "graphics.h"
#include <fcntl.h>
#include <sys/stat.h>
// #include <sys/time.h>
// #undef BITMAPINFO
#include <windows.h>

#include <time.h> //时间
static SDL_Renderer *renderer;
static SDL_Window *window;
int SCRH = 300;
int SCRW = 300;




//初始化
void base_init( SDL_Window *win, SDL_Renderer *render){
    renderer = render;
    window = win;
    SCRW = 480;
    SCRH = 720;
    setscrsize(SCRW,SCRH);
    // SDL_SetWindowDisplayMode(win, SDL_DisplayMode)
    // SDL_SetWindowPosition(win, 0,0);
}

//请屏
void cls(int r,int g,int b){
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);
}

//刷新屏幕
void ref(int x,int y,int w,int h){
    SDL_RenderPresent(renderer);
}

void capp_exit(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
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
    SDL_SetRenderDrawColor(renderer, r,g,b,255);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect); //
}

void dline(int x1,int x2,int y1,int y2,int r, int g,int b){
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void dpointex(int x,int y,int r,int g,int b){
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderDrawPoint(renderer,x,y);
}

void getscrsize(int *w,int *h){
    *w = SCRW;
    *h = SCRH;
}

void setscrsize(int w,int h){
    SCRW = w;
    SCRH = h;
    SDL_SetWindowSize(window, SCRW,SCRH);
    SDL_UpdateWindowSurface(window);
}

void effsetcon(int x,int y,int w,int h){

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
