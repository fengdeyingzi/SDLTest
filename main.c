#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "graphics.h"
#include <windows.h>

#include "capp.h"
#include "base.h"

/*
  使用SDL实现手机CAPP的绘图函数
  风的影子
*/

// LRESULT WINAPI WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
// char *win_main()
// {
//     int nShowCmd = 0;
//     int hInstance = 0;
//     TCHAR *szName=("myWindow");
//     HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, szName, ("我的窗口我喜欢"), WS_OVERLAPPEDWINDOW, 200, 100, 600, 400, NULL, NULL, hInstance, NULL); //创建窗口，窗口标题为"我的窗口我喜欢"
//     MSG Msg = {0};
//     ShowWindow(hWnd, nShowCmd); //显示窗口
//     UpdateWindow(hWnd);
//     //下面是对消息的循环处理，大家先不必管这些，下节课我会细说的
//     while (GetMessage(&Msg, NULL, 0, 0))
//     {
//         TranslateMessage(&Msg); //翻译消息
//         DispatchMessage(&Msg);  //分派消息
//     }
//     return Msg.message;
// }
// //消息处理函数
// LRESULT WINAPI WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
// {
//     switch (Msg) //处理消息过程
//     {
//     case WM_DESTROY:        //响应鼠标单击关闭按钮事件
//         PostQuitMessage(0); //退出消息队列
//         return 0;           //退出函数
//     }
//     return DefWindowProc(hWnd, Msg, wParam, lParam);
// }

int RINTERVALS = 1000;

int isDown = 0;
SDL_Window *window;
SDL_Surface *surface_window = NULL;
SDL_Surface *surface_ui = NULL;
SDL_Surface *surface_cache = NULL;
int main(int argc, char *argv[])
{
win_main();
    printf("start\n");
    //初始化所有SDL子系统
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        return -1;
    }
    window = SDL_CreateWindow("SDL测试 - 风的影子", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 720, SDL_WINDOW_SHOWN);
    SDL_Surface *icon = IMG_Load("assets/ic_launcher.png");
    SDL_SetWindowIcon(window, icon);
    surface_window = SDL_GetWindowSurface(window);
    int rmask = 0xFF000000;
    int gmask = 0x00FF0000;
    int bmask = 0x0000FF00;
    int amask = 0x000000FF; // RGBA8888模式
    surface_cache = SDL_CreateRGBSurface(SDL_PREALLOC, surface_window->w, surface_window->h, 32, rmask, gmask, bmask, amask);
    surface_ui = SDL_CreateRGBSurface(SDL_PREALLOC, surface_window->w, surface_window->h, 32, rmask, gmask, bmask, amask);
    bool quit = false;
    SDL_Event e;
    SDL_Point point;

    base_init();
    graphics_init();
    init(); //调用CAPP的init函数
    _TIMER *timer = NULL;
    Sint32 key = 0;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_USEREVENT:
                if (!quit)
                {
                    if (e.user.code == SDL_TIMER)
                    {
                        timer = (_TIMER *)(e.user.data1);
                        timer->timerCB(timer->data);
                    }
                }

                break;
            case SDL_QUIT:
                exitApp();
                quit = true;
                break;
            case SDL_KEYDOWN:
                printf("key:%d\n", e.key.keysym.sym);
                //判断基本按键事件
                key = e.key.keysym.sym;
                if (key == 1073741906) //up
                {
                    event(KY_DOWN, _UP, 0);
                }
                else if (key == 1073741905) //down
                {

                    event(KY_DOWN, _DOWN, 0);
                }
                else if (key == 1073741904) //left
                {

                    event(KY_DOWN, _LEFT, 0);
                }
                else if (key == 1073741903) //right
                {

                    event(KY_DOWN, _RIGHT, 0);
                }
                else if (key == 13)
                { //enter ok
                    event(KY_DOWN, _SELECT, 0);
                }
                else if (key == 27)
                { //esc back
                    event(KY_DOWN, _BACK, 0);
                }
                else if (key == 1073742048)
                { //ctrl menu
                    event(KY_DOWN, _MENU, 0);
                }
                else if (key == 48)
                { //0
                    event(KY_DOWN, _0, 0);
                }
                else if (key == 49)
                { //1
                    event(KY_DOWN, _1, 0);
                }
                else if (key == 50)
                { //2
                    event(KY_DOWN, _2, 0);
                }
                else if (key == 51)
                { //3
                    event(KY_DOWN, _3, 0);
                }
                else if (key == 52)
                { //4
                    event(KY_DOWN, _4, 0);
                }
                else if (key == 53)
                { //5
                    event(KY_DOWN, _5, 0);
                }
                else if (key == 54)
                { //6
                    event(KY_DOWN, _6, 0);
                }
                else if (key == 55)
                { //7
                    event(KY_DOWN, _7, 0);
                }
                else if (key == 56)
                { //8
                    event(KY_DOWN, _8, 0);
                }
                else if (key == 57)
                { //9
                    event(KY_DOWN, _9, 0);
                }
                break;
            case SDL_KEYUP:
                printf("key:%d\n", e.key.keysym.sym);
                //判断基本按键事件
                key = e.key.keysym.sym;
                if (key == 1073741906) //up
                {
                    event(KY_UP, _UP, 0);
                }
                else if (key == 1073741905) //down
                {

                    event(KY_UP, _DOWN, 0);
                }
                else if (key == 1073741904) //left
                {

                    event(KY_UP, _LEFT, 0);
                }
                else if (key == 1073741903) //right
                {

                    event(KY_UP, _RIGHT, 0);
                }
                else if (key == 13)
                { //enter ok
                    event(KY_UP, _SELECT, 0);
                }
                else if (key == 27)
                { //esc back
                    event(KY_UP, _BACK, 0);
                }
                else if (key == 1073742048)
                { //ctrl menu
                    event(KY_UP, _MENU, 0);
                }
                else if (key == 48)
                { //0
                    event(KY_UP, _0, 0);
                }
                else if (key == 49)
                { //1
                    event(KY_UP, _1, 0);
                }
                else if (key == 50)
                { //2
                    event(KY_UP, _2, 0);
                }
                else if (key == 51)
                { //3
                    event(KY_UP, _3, 0);
                }
                else if (key == 52)
                { //4
                    event(KY_UP, _4, 0);
                }
                else if (key == 53)
                { //5
                    event(KY_UP, _5, 0);
                }
                else if (key == 54)
                { //6
                    event(KY_UP, _6, 0);
                }
                else if (key == 55)
                { //7
                    event(KY_UP, _7, 0);
                }
                else if (key == 56)
                { //8
                    event(KY_UP, _8, 0);
                }
                else if (key == 57)
                { //9
                    event(KY_UP, _9, 0);
                }

                break;
            case SDL_MOUSEMOTION:
                if (isDown)
                    event(MS_MOVE, e.motion.x, e.motion.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                event(MS_DOWN, e.motion.x, e.motion.y);
                isDown = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                isDown = 0;
                event(MS_UP, e.motion.x, e.motion.y);
                break;
            }
        }

        // SDL_RenderPresent(renderer);
        //更新窗口操作
        // SDL_UpdateWindowSurface(window);
        // SDL_Delay(1000 / 60);
    }
    graphics_free();
    SDL_FreeSurface(surface_cache);
    SDL_FreeSurface(surface_window);
    SDL_DestroyWindow(window);
    // SDL_DestroyRenderer(renderer);

    return 0;
}
