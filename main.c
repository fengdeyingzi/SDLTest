#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
extern void drawImgEx(SDL_Renderer *renderer, char *file, int x, int y, int bx,int by,int bw,int bh);
extern void drawCircle(SDL_Renderer *renderer, SDL_Color *color, int x0, int y0, int r);
extern void drawRect(SDL_Renderer *renderer, int x, int y, int w, int h);
extern void drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2);
extern void drawPoint(SDL_Renderer *renderer, int x,int y);
extern Uint32 SDLColorToColor(SDL_Surface *screen,SDL_Color* sdl_color);
extern Uint32 get_pixel(SDL_Surface *surface, int x, int y);
extern void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int isDown = 0;
int main(int argc, char *argv[])
{
    SDL_Window *window = SDL_CreateWindow("demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Surface *surface = NULL;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool quit = false;
    SDL_Event event;
    SDL_Point point;
    //声明表面
    SDL_Surface *hello = NULL;
    SDL_Texture *pTexture = NULL;
//接下来是获取窗口的屏幕 这样才能绘制
    surface = SDL_GetWindowSurface(window);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    while (!quit)
    {
        Uint32 c_red = 0xffffffff;
        SDL_Rect rect = {0, 0, 20, 20};

        SDL_RenderFillRect(renderer, &rect); // 画圆，圆心（50，50） 半径50
                                             //加载图像
        hello = SDL_LoadBMP("ic_launcher.bmp");
        //使用SDL_Surface创建Texture
        pTexture = SDL_CreateTextureFromSurface(renderer, hello);
        SDL_Rect *srcRect = malloc(sizeof(SDL_Rect));
        SDL_Rect *dstRect = malloc(sizeof(SDL_Rect));
        srcRect->x = 0;
        srcRect->y = 0;
        srcRect->w = 140;
        srcRect->h = 140;
        dstRect->x = 0;
        dstRect->y = 0;
        dstRect->w = 140;
        dstRect->h = 140;
        SDL_RenderCopy(renderer, pTexture, srcRect, dstRect);
        SDL_Color color = {255,255,255,255};
        drawCircle(renderer, &color, 200,200, 50);
        drawLine(renderer,0,0,500,500);
        //将图像应用到窗口上
        // SDL_BlitSurface( hello, NULL, renderer, NULL );
        
        drawImgEx(renderer, "ic_launcher.bmp", 200,0,0,0,100,100);
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                if (isDown)
                {
                    SDL_RenderDrawLine(renderer, point.x, point.y, event.motion.x, event.motion.y);

                    point.x = event.motion.x;
                    point.y = event.motion.y;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                point.x = event.motion.x;
                point.y = event.motion.y;
                isDown = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                isDown = 0;
                break;
            }
        }
        SDL_RenderPresent(renderer);
        //更新窗口操作
        // SDL_UpdateWindowSurface(window);
        SDL_Delay(1000 / 60);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return 0;
}

//绘制一张图片
void drawImg(SDL_Renderer *renderer, char *file, int x, int y, int w, int h)
{
    SDL_Surface *hello = NULL;
    SDL_Texture *pTexture = NULL;
    hello = SDL_LoadBMP(file);
    //使用SDL_Surface创建Texture
    pTexture = SDL_CreateTextureFromSurface(renderer, hello);
    SDL_Rect *srcRect = malloc(sizeof(SDL_Rect));
    SDL_Rect *dstRect = malloc(sizeof(SDL_Rect));
    srcRect->x = 0;
    srcRect->y = 0;
    srcRect->w = w;
    srcRect->h = h;
    dstRect->x = x;
    dstRect->y = y;
    dstRect->w = w;
    dstRect->h = h;
    SDL_RenderCopy(renderer, pTexture, srcRect, dstRect);
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
    //用于保存坐标
    SDL_Rect offset;
    
    //获得坐标
    offset.x = x;
    offset.y = y;
    
    //Blit操作
    SDL_BlitSurface( source, clip, destination, &offset );
}

//图片裁剪绘制
void drawImgEx(SDL_Renderer *renderer, char *file, int x, int y, int bx,int by,int bw,int bh){
    //  SDL_Surface *hello = NULL;
    //  hello = SDL_LoadBMP(file);
    //  SDL_Rect clip = {bx,by,bw,bh};
    //  apply_surface(x, y, hello, screen, &clip);
        SDL_Surface *hello = NULL;
    SDL_Texture *pTexture = NULL;
    hello = SDL_LoadBMP(file);
    //使用SDL_Surface创建Texture
    pTexture = SDL_CreateTextureFromSurface(renderer, hello);
    SDL_Rect *srcRect = malloc(sizeof(SDL_Rect));
    SDL_Rect *dstRect = malloc(sizeof(SDL_Rect));
    srcRect->x = bx;
    srcRect->y = by;
    srcRect->w = bw;
    srcRect->h = bh;
    dstRect->x = x;
    dstRect->y = y;
    dstRect->w = bw;
    dstRect->h = bh;
    SDL_RenderCopy(renderer, pTexture, srcRect, dstRect);
}

//画圆
void drawCircle(SDL_Renderer *renderer, SDL_Color *color, int x0, int y0, int r)
{
    int x, y;
    int d;
    x = 0;
    y = r;
    d = 1 - r;
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    while (x < y)
    {
        SDL_RenderDrawPoint(renderer, x + x0, y + y0);
        SDL_RenderDrawPoint(renderer, x + x0, -y + y0);
        SDL_RenderDrawPoint(renderer, -x + x0, y + y0);
        SDL_RenderDrawPoint(renderer, -x + x0, -y + y0);
        SDL_RenderDrawPoint(renderer, y + x0, x + y0);
        SDL_RenderDrawPoint(renderer, y + x0, -x + y0);
        SDL_RenderDrawPoint(renderer, -y + x0, x + y0);
        SDL_RenderDrawPoint(renderer, -y + x0, -x + y0);
        if (d > 0)
        {
            d += ((x - y) << 1) + 5;
            y--;
        }
        else
        {
            d += (x << 1) + 3;
            //d += 2;
        }
        x++;
    }
}
//画矩形
void drawRect(SDL_Renderer *renderer, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};

    SDL_RenderFillRect(renderer, &rect); // 画圆，圆心（50，50） 半径50
}

//画线
void drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2)
{
    SDL_Rect rect = {x1, y1, x2, y2};

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2); // 画圆，圆心（50，50） 半径50
}

//画点
void drawPoint(SDL_Renderer *renderer, int x,int y){
    SDL_RenderDrawPoint(renderer, x, y);
}
//画文字



/**
 * Name : SDLColorToColor
 *
 * Change from an "SDL_Color" to an Uint32
 * sdl_color input SDL_Color Color
 * return Uint32 Color
 */
Uint32 SDLColorToColor(SDL_Surface *screen,SDL_Color* sdl_color)
{
	return  SDL_MapRGB(screen->format, sdl_color->r, sdl_color->g, sdl_color->b);
}
 
/*
 * Return the pixel value at (x, y) 获取像素
 * NOTE: The surface must be locked before calling this!
 */
Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
 
    switch(bpp) {
    case 1:
        return *p;
 
    case 2:
        return *(Uint16 *)p;
 
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
 
    case 4:
        return *(Uint32 *)p;
 
    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}
 
/*
 * Set the pixel at (x, y) to the given value 设置像素
 * NOTE: The surface must be locked before calling this!
 */
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    switch (surface->format->BytesPerPixel)
  	{
    	case 1: // Assuming 8-bpp
    	{
      		Uint8 *bufp;
      		bufp = (Uint8 *)surface->pixels + y*surface->pitch + x;
      		*bufp = pixel;
    	}
    	break;
    	case 2: // Probably 15-bpp or 16-bpp
    	{
      		Uint16 *bufp;
      		bufp = (Uint16 *)surface->pixels + y*surface->pitch/2 + x;
      		*bufp = pixel;
    	}
    	break;
    	case 3: // Slow 24-bpp mode, usually not used
    	{
      		Uint8 *bufp;
      		bufp = (Uint8 *)surface->pixels + y*surface->pitch + x * 3;
      		if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
      		{
        		bufp[0] = pixel;
        		bufp[1] = pixel >> 8;
        		bufp[2] = pixel >> 16;
      		} else {
        		bufp[2] = pixel;
        		bufp[1] = pixel >> 8;
        		bufp[0] = pixel >> 16;
      		}
    	}
    	break;
    	case 4: // Probably 32-bpp
    	{
      		Uint32 *bufp;
      		bufp = (Uint32 *)surface->pixels + y*surface->pitch/4 + x;
      		*bufp = pixel;
    	}
    	break;
  	}
}
