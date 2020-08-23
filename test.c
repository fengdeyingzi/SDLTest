#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "base.h"
#include "utils.h"
#include "graphics.h"

/*
  使用SDL实现手机CAPP的绘图函数
  风的影子
*/

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

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip)
{
    //用于保存坐标
    SDL_Rect offset;

    //获得坐标
    offset.x = x;
    offset.y = y;

    //Blit操作
    SDL_BlitSurface(source, clip, destination, &offset);
}
int isDown = 0;
int main(int argc, char *argv[])
{
    printf("start\n");
    //初始化所有SDL子系统
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return -1;    
    }
    SDL_Window *window = SDL_CreateWindow("SDL测试 - 风的影子", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 720, SDL_WINDOW_SHOWN);
   SDL_Surface *hello = IMG_Load("assets/ic_launcher.png");
    
    // SDL_Surface *surface = NULL;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    bool quit = false;
    SDL_Event e;
    SDL_Point point;
    
    //接下来是获取窗口的屏幕 这样才能绘制
    SDL_Surface *surface_screen = SDL_GetWindowSurface(window);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    int rmask = 0xFF000000; int gmask = 0x00FF0000; int bmask = 0x0000FF00; int amask = 0x000000FF;	// RGBA8888模式
    int w = 300,h=300;
    SDL_Surface *surface_cache = SDL_CreateRGBSurface(SDL_PREALLOC, w, h , 32, rmask, gmask, bmask, amask);
    SDL_Renderer *renderer_cache = SDL_CreateSoftwareRenderer(surface_cache);
    // SDL_Surface *surface_cache = SDL_CreateRGBSurface(SDL_PREALLOC, w, h , 32, rmask, gmask, bmask, amask);
    graphics_init(window,renderer);
    base_init(window,renderer);
    surface_screen = SDL_GetWindowSurface(window);
    renderer = base_getRenderer();
    uint32 color = 0xffffffff;
    SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
   
    SDL_Texture *texture_screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, 300, 300);
    SDL_Texture *hello_texture = SDL_CreateTextureFromSurface(renderer, hello);
    
    char temp[300];
      SDL_Rect srcrect = {0,0,100,30};
    SDL_Rect dstrect = {0,0,100,30};
    unsigned short *pixels = (uint16*)malloc(300*300*2);
    for(int n=0;n<300*300;n++){
        *(pixels+n) = MAKERGB565(255,0,0);
    }
    SDL_UpdateTexture(texture_screen,&srcrect, pixels, 300*2 );
    SDL_RenderCopy(renderer_cache, hello_texture, &srcrect ,&srcrect);
    // SDL_GetRenderTarget(texture_screen);
    SDL_Renderer *renderer_screen = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderTarget(renderer_screen,texture_screen);
    SDL_RenderCopy(renderer_screen, hello_texture, &srcrect, &dstrect);
    SDL_RenderPresent(renderer_screen);
    SDL_RenderPresent(renderer);
    // SDL_RenderFillRect(renderer, &srcrect); //
    // sprintf(temp,"render:%d\n", SDL_GetTextureAlphaMod());
    // LOG_I(temp);
  

    // for(int i=0;i<200;i++){
    //     Uint32 color = get_pixel(surface_cache,i,i);
    //     put_pixel(surface_screen,i,i,color);
    // }
    
    
  
// 
// drawRect(0,0,30,30,0xfff0f0f0);
    apply_surface(0,0,  hello,  surface_cache, &dstrect);
      for(int i=0;i<300;i++){
        put_pixel(surface_screen,i,30,0xff0000ff);
        // put_pixel(surface_screen,i,i,0x55555555);
    }
    apply_surface(200,10, surface_cache, surface_screen, &srcrect  );
SDL_UpdateWindowSurface(window);
//  SDL_RenderCopy(renderer, texture_screen, &srcrect, &dstrect);
    while (!quit)
    {
        // drect(0,0,300,300,255,0,0);
        // ref(0,0,300,300);
        // drect(0,0,300,300,255,255,255);
        // dline(0,0,300,300,255,0,0);
        
        // ref(0,0,20,30);
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                if(isDown)
                {
                    
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                
                isDown = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                isDown = 0;
                
                break;
            }
        }
       SDL_UpdateWindowSurface(window);
        // SDL_RenderPresent(renderer);
        
        //更新窗口操作
        
        // ref(0,0,30,30);
        SDL_Delay(1000);
    }
    
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);


return 0;
}
