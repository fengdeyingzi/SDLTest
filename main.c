#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int isDown = 0;
int main(int argc, char *argv[])
{
    SDL_Window *window=SDL_CreateWindow("demo",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    bool quit=false;
    SDL_Event event;
    SDL_Point point;
     //声明表面
    SDL_Surface* hello = NULL;
    SDL_Texture* pTexture = NULL;

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    while(!quit){
         Uint32 c_red = 0xffffffff;
         SDL_Rect rect = {0,0,20,20};
         
         SDL_RenderFillRect(renderer, &rect); // 画圆，圆心（50，50） 半径50
          //加载图像
    hello = SDL_LoadBMP( "ic_launcher.bmp" );
    //使用SDL_Surface创建Texture
	pTexture = SDL_CreateTextureFromSurface(renderer, hello);
    SDL_Rect *srcRect = malloc(sizeof(SDL_Rect));
    SDL_Rect *dstRect = malloc(sizeof(SDL_Rect));
    srcRect->x=0;
    srcRect->y = 0;
    srcRect->w = 140;
    srcRect->h = 140;
    dstRect->x=0;
    dstRect->y = 0;
    dstRect->w = 140;
    dstRect->h = 140;
    SDL_RenderCopy(renderer, pTexture, srcRect, dstRect);
    //将图像应用到窗口上
    // SDL_BlitSurface( hello, NULL, renderer, NULL );
    //更新Render显示
	SDL_RenderPresent(renderer);
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    quit=true;
                    break;
                case SDL_MOUSEMOTION:
                    if(isDown){
                    SDL_RenderDrawLine(renderer,point.x,point.y,event.motion.x,event.motion.y);
                    
                    point.x=event.motion.x;
                    point.y=event.motion.y;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    point.x=event.motion.x;
                    point.y=event.motion.y;
                    isDown = 1;
                break;
                case SDL_MOUSEBUTTONUP:
                    isDown = 0;
                break;
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(1000/60);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return 0;
}

