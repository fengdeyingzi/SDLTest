
#ifndef __BASE_H__
#define __BASE_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

//系统版本，由于软件的更新，可能会造成有些函数、宏、类型等不一致而导致运行出错
//建议在源码中判断系统版本是否相同
#define _VERSION 3100

typedef unsigned short uint16;    //有符号16bit整型
typedef unsigned long int uint32; //无符号32bit整型
typedef long int int32;           //有符号32bit整型
typedef unsigned char uint8;      //无符号8bit整型
typedef signed char int8;         //有符号8bit整型
typedef signed short int16;       //有符号16bit整型

//下面四项为大部分系统函数的返回值，没有在系统内定义，如果需要这些宏定义请在其他头文件中定义
#define MR_SUCCESS 0 //成功
#define MR_FAILED -1 //失败
#define MR_IGNORE 1  //不关心
#define MR_WAITING 2 //异步(非阻塞)模式

// #define NULL (void *)0
#define TRUE 1
#define FALSE 0
extern int SCRW ;//系统内部宏，值为屏幕宽
extern int SCRH ;//系统内部宏，值为屏幕高
#define TCC 1

//基本按键值（未定义的其他按键也可以使用，但需知道其键值）
enum
{
    _0,          //按键 0
    _1,          //按键 1
    _2,          //按键 2
    _3,          //按键 3
    _4,          //按键 4
    _5,          //按键 5
    _6,          //按键 6
    _7,          //按键 7
    _8,          //按键 8
    _9,          //按键 9
    _STAR,       //按键 *
    _POUND,      //按键 #
    _UP,         //按键 上
    _DOWN,       //按键 下
    _LEFT,       //按键 左
    _RIGHT,      //按键 右
    _SLEFT = 17, //按键 左软键
    _SRIGHT,     //按键 右软键
    _MENU = 17,  //菜单键
    _BACK,       //返回键
    _SEND,       //按键 接听键
    _SELECT      //按键 确认/选择（若方向键中间有确认键，建议设为该键）
};

//基本事件（其他事件需自己定义）
enum
{
    KY_DOWN,     //按键按下
    KY_UP,       //按键释放
    MS_DOWN,     //鼠标按下
    MS_UP,       //鼠标释放
    MN_SLT,      //菜单选择
    MN_RET,      //菜单返回
    MR_DIALOG,   //对话框
    MS_MOVE = 12 //鼠标移动
};

enum
{
    DLG_OK,    //对话框/文本框等的"确定"键被点击(选择)
    DLG_CANCEL //对话框/文本框等的"取消"("返回")键被点击(选择)
};

// enum
// {
//     SEEK_SET, //从文件起始开始
//     SEEK_CUR, //从当前位置开始
//     SEEK_END  //从文件末尾开始
// };
enum
{
    IS_FILE = 1,    //文件
    IS_DIR = 2,     //目录
    IS_INVALID = 8, //无效(非文件、非目录)
};

typedef struct
{
    uint16 x;
    uint16 y;
    uint16 w;
    uint16 h;
} rectst;

typedef struct
{
    uint8 r;
    uint8 g;
    uint8 b;
} colorst;

typedef void (*mrc_timerCB)(int data);
typedef struct MRC_TIMER {
    int32 time;    //定时器时长
    int32 uptime;  //剩余时间(无效)
    uint32 starttime; //定时器创建时间(无效)
    int32 data;  //data数据
    mrc_timerCB timerCB;  //回调函数
    int32 loop;  //是否循环
    int32 isrun;  //是否在运行(无效，被runnable取代)
    struct MRC_TIMER *next;  //指向下一个定时器
    int32 isStop;
    SDL_TimerID timerId;
    SDL_TimerCallback callback;
} _TIMER;
#define SDL_TIMER 0x8001


void base_init( SDL_Window *win, SDL_Renderer *render);

//请屏
void cls(int r,int g,int b);

//刷新屏幕
void ref(int x,int y,int w,int h);

void capp_exit();
int getlen(const char* filename);
int wstrlen(const char * txt);
int timercreate();
int timerstart(int t, int time, int data, mrc_timerCB timerCB, int loop);
int timerstop(int t);
int timerdel(int t);
void sleep(int ms);
unsigned int getuptime();
void drect(int x,int y,int w,int h,int r,int g,int b);
void dline(int x1,int x2,int y1,int y2,int r, int g,int b);
void dpointex(int x,int y,int r,int g,int b);
void getscrsize(int *w,int *h);
void setscrsize(int w,int h);

#endif
