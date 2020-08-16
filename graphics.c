/*
SDL 实现手机C的graphics.h
*/

#include "graphics.h"
#include "base.h"


static SDL_Renderer *renderer;
static SDL_Window *window;
static TTF_Font *fonts[3];

int createBitmap(int w, int h);

static int min(int num1,int num2){
    return num1<num2? num1 : num2;
}

void graphics_init(SDL_Window *win,SDL_Renderer *render)
{
    renderer = render;
    window = win;
    //初始化SDL_ttf
    if (TTF_Init() == -1)
    {
        printf("ttf_init error\n");
    }
    fonts[0] = TTF_OpenFont("assets/font.ttf", 16);
    fonts[1] = TTF_OpenFont("assets/font.ttf", 20);
    fonts[2] = TTF_OpenFont("assets/font.ttf", 24);
    if (!fonts[0])
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
    if (!fonts[1])
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
    if (!fonts[2])
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
    //设置支持 Alpha
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}
void setDrawRenderer(SDL_Renderer *render)
{
    renderer = render;
}

void drawText(char *text, int x, int y,unsigned int color, int is_unicode, int font_type)
{
    //打开字库
    // TTF_Font *font;
    // font=TTF_OpenFont("font.ttf", 64);
    // if(!font) {
    //     printf("TTF_OpenFont: %s\n", TTF_GetError());
    // }
    SDL_Color scolor = {(color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff}; //颜色
    SDL_Surface *text_surface;
    SDL_Texture *pTexture = NULL;
    if(is_unicode){
        text_surface = TTF_RenderUNICODE_Solid(fonts[font_type], (Uint16*)text, scolor);
    }
    else
    text_surface = TTF_RenderUTF8_Solid(fonts[font_type], text, scolor);
    if (text_surface != NULL)
    {
        pTexture = SDL_CreateTextureFromSurface(renderer, text_surface);
        // SDL_FreeSurface(text_surface);
        SDL_Rect srcrect = {0, 0, text_surface->w, text_surface->h};
        SDL_Rect dstrect = {x, y, text_surface->w, text_surface->h};
        //往渲染器上copy字体
        //<-----------------第二步：渲染文字即可----------------------------->
        SDL_RenderCopy(renderer, pTexture, &srcrect, &dstrect);
        SDL_DestroyTexture(pTexture);
        printf("drawText 1\n");
        // drawBitmap(x,y, (int)text_surface);
        // SDL_BlitSurface(text_surface, NULL, renderer, NULL);//将文字复制到屏幕的surface上面
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
}

void drawTextEx(char *text, int x,int y, rectst *rect, unsigned int color, int is_unicode, int font_type)
{
    //打开字库
    // TTF_Font *font;
    // font=TTF_OpenFont("font.ttf", 64);
    // if(!font) {
    //     printf("TTF_OpenFont: %s\n", TTF_GetError());
    // }
    SDL_Color scolor = {(color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff}; //颜色
    SDL_Surface *text_surface;
    SDL_Texture *pTexture = NULL;
    if(is_unicode){
        text_surface = TTF_RenderUNICODE_Blended_Wrapped(fonts[font_type], (Uint16*)text, scolor, rect->w);
    }
    else
    text_surface = TTF_RenderUTF8_Blended_Wrapped(fonts[font_type], text, scolor, rect->w);
    SDL_Rect cliprect = {rect->x, rect->y, rect->w, rect->h};
    int window_w,window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);
    SDL_Rect allrect = {0, 0, window_w, window_h};
    if (text_surface != NULL)
    {
        pTexture = SDL_CreateTextureFromSurface(renderer, text_surface);
        // SDL_FreeSurface(text_surface);
        int dw = min(rect->w, text_surface->w);
        int dh = min(rect->h, text_surface->h);
        SDL_Rect srcrect = {0, 0, text_surface->w, text_surface->h};
        SDL_Rect dstrect = {x, y, text_surface->w, text_surface->h};
        SDL_RenderSetClipRect(renderer, &cliprect);
        //往渲染器上copy字体
        //<-----------------第二步：渲染文字即可----------------------------->
        SDL_RenderCopy(renderer, pTexture, &srcrect, &dstrect);
        SDL_DestroyTexture(pTexture);
        
        printf("drawText 1\n");
        // drawBitmap(x,y, (int)text_surface);
        // SDL_BlitSurface(text_surface, NULL, renderer, NULL);//将文字复制到屏幕的surface上面
        SDL_FreeSurface(text_surface);
        SDL_RenderSetClipRect(renderer, &allrect);
        printf("drawText ok\n");
        return;
    }
    else
    {
        // report error
        printf("drawText error\n");
        return;
    }
}

void getTextWH(char *text,int is_unicode, int font_type, int *w, int *h){
    if(is_unicode){
        TTF_SizeUNICODE(fonts[font_type], (Uint16*)text, w, h);
    }
    else
    TTF_SizeUTF8(fonts[font_type], text, w,h);
}

int readBitmap(char *filename)
{
    SDL_Surface *surface = NULL;
    SDL_Texture *pTexture = NULL;

    // surface = SDL_LoadBMP(filename);
    surface = IMG_Load(filename);
    // //使用SDL_Surface创建Texture
    // pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    return (int)surface;
}

int readBitmapFromAssets(char *filename)
{
    SDL_Surface *surface = NULL;
    SDL_Texture *pTexture = NULL;
    char text[300];
    sprintf(text, "assets\\%s", filename);
    surface = IMG_Load(text);

    // //使用SDL_Surface创建Texture
    // pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    return (int)surface;
}

void drawBitmap(int bmp, int x, int y)
{
    SDL_Surface *surface = (SDL_Surface *)bmp;
    SDL_Texture *pTexture = NULL;
    SDL_Rect srcRect = {0, 0, surface->w, surface->h};
    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, pTexture, &srcRect, &dstRect);
    SDL_DestroyTexture(pTexture);
}

void drawBitmapFlip(int bmp, int x, int y, int w, int h, int tx, int ty)
{
    SDL_Surface *surface = (SDL_Surface *)bmp;
    SDL_Texture *pTexture = NULL;
    SDL_Rect srcRect = {tx, ty, w, h};
    SDL_Rect dstRect = {x, y, w, h};
    pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, pTexture, &srcRect, &dstRect);
    SDL_DestroyTexture(pTexture);
}

void drawBitmapEx(int bmp, int x, int y, int w, int h, int tx, int ty, int tw, int th)
{
    SDL_Surface *surface = (SDL_Surface *)bmp;
    SDL_Texture *pTexture = NULL;
    SDL_Rect srcRect = {tx, ty, tw, th};
    SDL_Rect dstRect = {x, y, w, h};
    pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, pTexture, &srcRect, &dstRect);
    SDL_DestroyTexture(pTexture);
}

//释放已加载的图像
void bitmapFree(int bmp)
{
    SDL_Surface *surface = (SDL_Surface *)bmp;
    SDL_FreeSurface(surface);
}

//
void drawPoint(int x,int y,unsigned int color){
    SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
    SDL_RenderDrawPoint(renderer,x,y);
}

//画矩形
void drawRect(int x, int y, int w, int h, unsigned int color)
{
    SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect); //
}

void drawCircle(int x, int y, int radius, unsigned int color)
{
    // using std::cout;
    // using std::clock;
    // using std::endl;
    // int st=clock();
    int i = 0;
    int tx = 0, ty = radius, d = 3 - (radius << 1);
    SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
    while (tx < ty)
    {
        for (i = x - ty; i <= x + ty; ++i)
        {
            SDL_RenderDrawPoint(renderer, i, y - tx);
            if (tx)
                SDL_RenderDrawPoint(renderer, i, y + tx);
        }
        if (d < 0)
            d += (tx << 2) + 6;
        else
        {
            for (i = x - tx; i <= x + tx; ++i)
            {
                SDL_RenderDrawPoint(renderer, i, y - ty);
                SDL_RenderDrawPoint(renderer, i, y + ty);
            }
            d += ((tx - ty) << 2) + 10, ty--;
        }
        tx++;
    }
    if (tx == ty)
        for (i = x - ty; i <= x + ty; ++i)
        {
            SDL_RenderDrawPoint(renderer, i, y - tx);
            SDL_RenderDrawPoint(renderer, i, y + tx);
        }
    // int en=clock();
    // cout<<"Drawing Circle(radius "<<radius<<" pixels),costs:"<<en-st<<"ms"<<endl;
}

void drawLine(int x1, int y1, int x2, int y2, unsigned int color)
{
    SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void graphics_free(){
    int i=0;
    //关闭用过的字体
    for(i=0;i<3;i++)
    TTF_CloseFont( fonts[i] );
    
    //退出SDL_ttf
    TTF_Quit();
}


