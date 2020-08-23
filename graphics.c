/*
SDL 实现手机C的graphics.h
*/

#include "graphics.h"
#include "base.h"
#include "utils.h"


extern SDL_Surface *surface_window;
extern SDL_Surface *surface_cache;
static TTF_Font *fonts[3];

int createBitmap(int w, int h){
    int rmask = 0xFF000000; int gmask = 0x00FF0000; int bmask = 0x0000FF00; int amask = 0x000000FF;	// RGBA8888模式
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h , 32, rmask, gmask, bmask, amask);
    return (int)surface;
}

static int min(int num1,int num2){
    return num1<num2? num1 : num2;
}

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

void graphics_init()
{

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
  
    if(is_unicode){
        text_surface = TTF_RenderUNICODE_Solid(fonts[font_type], (Uint16*)text, scolor);
    }
    else
    text_surface = TTF_RenderUTF8_Solid(fonts[font_type], text, scolor);
    if (text_surface != NULL)
    {
       
        // SDL_FreeSurface(text_surface);
        SDL_Rect srcrect = {0, 0, text_surface->w, text_surface->h};
        SDL_Rect dstrect = {x, y, text_surface->w, text_surface->h};
        //往渲染器上copy字体
        //<-----------------第二步：渲染文字即可----------------------------->
        // SDL_RenderCopy(renderer, pTexture, &srcrect, &dstrect);
        // SDL_DestroyTexture(pTexture);
        printf("drawText 1\n");
        SDL_BlitSurface(text_surface, &srcrect, surface_cache, &dstrect);//将文字复制到屏幕的surface上面
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
    // SDL_Texture *pTexture = NULL;
    if(is_unicode){
        text_surface = TTF_RenderUNICODE_Blended_Wrapped(fonts[font_type], (Uint16*)text, scolor, rect->w);
    }
    else
    text_surface = TTF_RenderUTF8_Blended_Wrapped(fonts[font_type], text, scolor, rect->w);
    SDL_Rect cliprect = {rect->x, rect->y, rect->w, rect->h};
    int window_w,window_h;
    
    SDL_Rect allrect = {0, 0, window_w, window_h};
    if (text_surface != NULL)
    {
        // pTexture = SDL_CreateTextureFromSurface(renderer, text_surface);
        // SDL_FreeSurface(text_surface);
        int dw = min(rect->w, text_surface->w);
        int dh = min(rect->h, text_surface->h);
        SDL_Rect srcrect = {0, 0, text_surface->w, text_surface->h};
        SDL_Rect dstrect = {x, y, text_surface->w, text_surface->h};
        // SDL_RenderSetClipRect(renderer, &cliprect);
        //往渲染器上copy字体
        //<-----------------第二步：渲染文字即可----------------------------->
        // SDL_RenderCopy(renderer, pTexture, &srcrect, &dstrect);
        // SDL_DestroyTexture(pTexture);
        SDL_BlitSurface(text_surface, &srcrect, surface_cache, &dstrect);//将文字复制到屏幕的surface上面
        printf("drawText 1\n");
        // drawBitmap(x,y, (int)text_surface);
        // SDL_BlitSurface(text_surface, NULL, renderer, NULL);//将文字复制到屏幕的surface上面
        SDL_FreeSurface(text_surface);
        // SDL_RenderSetClipRect(renderer, &allrect);
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
    if(strncmp(filename,"assets://",9)==0){
        return readBitmapFromAssets(filename+9);
    }
    surface = IMG_Load(filename);
    // //使用SDL_Surface创建Texture
    // pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    return (int)surface;
}

int clipBitmap(int bmp, int x,int y,int w,int h){
     SDL_Surface *surface = (SDL_Surface *)bmp;
     SDL_Surface *newsurface = (SDL_Surface *)createBitmap(w,h);
     SDL_Rect srcrect = {x,y,w,h};
     SDL_Rect dstrect = {0,0,w,h};

     SDL_UpperBlit(surface,  &srcrect, newsurface, &dstrect);
     
     return (int)newsurface;
}

int drawBitmapOld(int di, int buf, int x,int y,int w,int h, int sx, int sy){
    SDL_Surface *surface_di = (SDL_Surface *)di;
    SDL_Surface *surface_buf = (SDL_Surface *)buf;
    SDL_Rect srcrect = {sx,sy,w,h};
    SDL_Rect dstrect = {x,y,w,h};

    SDL_UpperBlit(surface_buf,  &srcrect, surface_di, &dstrect);
    return 0;
}

int saveBitmap(int bmp, char *filename, int type, int load){
    return 0;
}

int bitmapGetInfo(int bmp, _BITMAPINFO *info){
    SDL_Surface *surface = (SDL_Surface *)bmp;
    info->width = surface->w;
    info->height = surface->h;
    info->format = surface->format->format;
    info->ptr = surface->pixels;
    return 0;
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
    // SDL_Texture *pTexture = NULL;
    SDL_Rect srcRect = {0, 0, surface->w, surface->h};
    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    // pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    // SDL_RenderCopy(renderer, pTexture, &srcRect, &dstRect);
    // SDL_DestroyTexture(pTexture);
    SDL_BlitSurface(surface, &srcRect, surface_cache, &dstRect);//将文字复制到屏幕的surface上面
}

void drawBitmapFlip(int bmp, int x, int y, int w, int h, int tx, int ty)
{
    SDL_Surface *surface = (SDL_Surface *)bmp;
    // SDL_Texture *pTexture = NULL;
    SDL_Rect srcRect = {tx, ty, w, h};
    SDL_Rect dstRect = {x, y, w, h};
    // pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    // SDL_RenderCopy(renderer, pTexture, &srcRect, &dstRect);
    // SDL_DestroyTexture(pTexture);
    SDL_BlitSurface(surface, &srcRect, surface_cache, &dstRect);//将文字复制到屏幕的surface上面
}

void drawBitmapEx(int bmp, int x, int y, int w, int h, int tx, int ty, int tw, int th)
{
    SDL_Surface *surface = (SDL_Surface *)bmp;
    // SDL_Texture *pTexture = NULL;
    SDL_Rect srcRect = {tx, ty, tw, th};
    SDL_Rect dstRect = {x, y, w, h};
    // pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    // SDL_RenderCopy(renderer, pTexture, &srcRect, &dstRect);
    // SDL_DestroyTexture(pTexture);
    SDL_BlitSurface(surface, &srcRect, surface_cache, &dstRect);//将文字复制到屏幕的surface上面
}

void drawBitmapRotate(int bmp, int scrx, int scry, int bx,int by, int r, int alpha){
    SDL_Surface *surface = (SDL_Surface *)bmp;
    // SDL_Texture *pTexture = NULL;
    SDL_Rect srcRect = {0, 0, surface->w, surface->h};
    SDL_Rect dstRect = {scrx, scry, surface->w, surface->h};
    // pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Point point = {bx,by};
    // SDL_RenderCopyEx(renderer, pTexture, &srcRect, &dstRect, r, &point, SDL_FLIP_NONE);
    // SDL_DestroyTexture(pTexture);
    // SDL_BlitSurface
}

//释放已加载的图像
void bitmapFree(int bmp)
{
    SDL_Surface *surface = (SDL_Surface *)bmp;
    SDL_FreeSurface(surface);
}

//
void drawPoint(int x,int y,unsigned int color){
    // SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
    // SDL_RenderDrawPoint(renderer,x,y);
    uint32 ncolor = (color<<8) | (color>>24);
    if(x>=0&&y>=0&&x<SCRW&&y<SCRH)
    put_pixel(surface_cache,x,y,ncolor);
}

//画矩形
void drawRect(int x, int y, int w, int h, unsigned int color)
{
    // SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
    // SDL_Rect rect = {x, y, w, h};
    // SDL_RenderFillRect(renderer, &rect); //
    for(int ix=0;ix<w;ix++){
        for(int iy=0;iy<h;iy++){
            if(ix>=0&&iy>=0&& ix<SCRW&& iy<SCRH)
            put_pixel(surface_cache,ix,iy,color);
        }
    }
}

void drawCircle(int x, int y, int radius, unsigned int color)
{
    // using std::cout;
    // using std::clock;
    // using std::endl;
    // int st=clock();
    int i = 0;
    int tx = 0, ty = radius, d = 3 - (radius << 1);
    // SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
    while (tx < ty)
    {
        for (i = x - ty; i <= x + ty; ++i)
        {
            put_pixel(surface_cache, i, y - tx, color);
            if (tx)
                put_pixel(surface_cache, i, y + tx, color);
        }
        if (d < 0)
            d += (tx << 2) + 6;
        else
        {
            for (i = x - tx; i <= x + tx; ++i)
            {
                put_pixel(surface_cache, i, y - ty, color);
                put_pixel(surface_cache, i, y + ty, color);
            }
            d += ((tx - ty) << 2) + 10, ty--;
        }
        tx++;
    }
    if (tx == ty)
        for (i = x - ty; i <= x + ty; ++i)
        {
            put_pixel(surface_cache, i, y - tx, color);
            put_pixel(surface_cache, i, y + tx, color);
        }
    // int en=clock();
    // cout<<"Drawing Circle(radius "<<radius<<" pixels),costs:"<<en-st<<"ms"<<endl;
}

void drawLine(int x1, int y1, int x2, int y2, unsigned int color)
{
    // SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, (color >> 24) & 0xff);
    // SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        	int x, y, dx, dy, c1, c2, err, swap = 0;
// uint32 color = (0xff000000|(r<<16)|(g<<8)|b);
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
	// nativecolor = MAKERGB(r, g, b);

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
		drawPoint((int16)(swap?y:x),(int16)(swap?x:y),color);
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

void graphics_free(){
    int i=0;
    //关闭用过的字体
    for(i=0;i<3;i++)
    TTF_CloseFont( fonts[i] );
    
    //退出SDL_ttf
    TTF_Quit();
}


