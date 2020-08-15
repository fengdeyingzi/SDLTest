/*
SDL 实现手机C的graphics.h
*/

#include "graphics.h"



static SDL_Renderer *renderer;

int createBitmap(int w, int h);

void setDrawRenderer(SDL_Renderer *render){
    renderer = render;
}

int readBitmap(char *filename){
    SDL_Surface *surface = NULL;
    SDL_Texture *pTexture = NULL;
    surface = SDL_LoadBMP(filename);
    // //使用SDL_Surface创建Texture
    // pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    return (int)surface;
}

int readBitmapFromAssets(char *filename){
    SDL_Surface *surface = NULL;
    SDL_Texture *pTexture = NULL;
    char text[300];
    sprintf(text,"assets\\%s",filename);
    surface = SDL_LoadBMP(text);
    // //使用SDL_Surface创建Texture
    // pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    return (int)surface;
}

void drawBitmap(int bmp, int x,int y){
    SDL_Surface *surface = (SDL_Surface*)bmp;
    SDL_Texture *pTexture = NULL;
    SDL_Rect srcRect = {0,0,surface->w, surface->h};
    SDL_Rect dstRect = {x,y,surface->w,surface->h};
    pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, pTexture, &srcRect, &dstRect);
}

void drawBitmapFlip(int bmp, int x,int y, int w,int h, int tx,int ty){
    SDL_Surface *surface = (SDL_Surface*)bmp;
    SDL_Texture *pTexture = NULL;
    SDL_Rect srcRect = {tx,ty,w, h};
    SDL_Rect dstRect = {x,y,w,h};
    pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, pTexture, &srcRect, &dstRect);
}

void drawBitmapEx(int bmp, int x,int y,int w,int h, int tx,int ty,int tw,int th){
    SDL_Surface *surface = (SDL_Surface*)bmp;
    SDL_Texture *pTexture = NULL;
    SDL_Rect srcRect = {tx,ty,tw, th};
    SDL_Rect dstRect = {x,y,w,h};
    pTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, pTexture, &srcRect, &dstRect);
}

//释放已加载的图像
void bitmapFree(int bmp){
    SDL_Surface *surface = (SDL_Surface*)bmp;
    SDL_FreeSurface( surface );
}
    

//画矩形
void drawRect(int x, int y, int w, int h, unsigned int color)
{
    SDL_SetRenderDrawColor(renderer, (color>>16)&0xff, (color>>8)&0xff,color&0xff, (color>>24)&0xff);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect); // 
}

void drawCircle(int x,int y,int radius, unsigned int color){
    // using std::cout;
    // using std::clock;
    // using std::endl;
    // int st=clock();
    int i=0;
    int tx=0,ty=radius,d=3-(radius<<1);
    SDL_SetRenderDrawColor(renderer, (color>>16)&0xff, (color>>8)&0xff,color&0xff, (color>>24)&0xff);
	while(tx<ty){
		for (i=x-ty;i<=x+ty;++i){
			SDL_RenderDrawPoint(renderer,i,y-tx);
			if (tx)
				SDL_RenderDrawPoint(renderer,i,y+tx);
		}
		if (d<0)
			d+=(tx<<2)+6;
		else{
			for (i=x-tx;i<=x+tx;++i){
                SDL_RenderDrawPoint(renderer,i,y-ty);
				SDL_RenderDrawPoint(renderer,i,y+ty);
			}
			d+=((tx - ty)<<2)+10,ty--;
		}
		tx++;
	}
	if (tx==ty)
		for (i=x-ty;i<=x+ty;++i){
                SDL_RenderDrawPoint(renderer,i,y-tx);
				SDL_RenderDrawPoint(renderer,i,y+tx);
		}
    // int en=clock();
    // cout<<"Drawing Circle(radius "<<radius<<" pixels),costs:"<<en-st<<"ms"<<endl;
}

void drawLine(int x1,int y1,int x2,int y2,unsigned int color){
    SDL_SetRenderDrawColor(renderer, (color>>16)&0xff, (color>>8)&0xff,color&0xff, (color>>24)&0xff);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}
