#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    SDL_Window *window=SDL_CreateWindow("demo",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    bool quit=false;
    SDL_Event event;
    SDL_Point point;

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    while(!quit){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    quit=true;
                    break;
                case SDL_MOUSEMOTION:
                    SDL_RenderDrawLine(renderer,point.x,point.y,event.motion.x,event.motion.y);
                    point.x=event.motion.x;
                    point.y=event.motion.y;
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

