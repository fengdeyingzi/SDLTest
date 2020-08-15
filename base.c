#include <stdio.h>
#include <SDL2/SDL.h>
#include "base.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <windows.h>
#include <time.h> //时间
static SDL_Renderer *renderer;
static SDL_Window *window;
//初始化
void base_init( SDL_Window *win, SDL_Renderer *render){
    renderer = render;
    window = win;
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

}

