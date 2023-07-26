#include <SDL2/SDL.h>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <cstdlib>

             constexpr int ww=200,
    wh=200,gw=50,gh=50;struct Pix{std::uint8_t
  r,g,b,a;};Pix pb={0,0,0,255};Pix pw={255,255,255,
  255};Pix cr={255,0,0,255};Pix cg={50,255,80,255};Pix
cb={0xea,0xad,0x79,255};Pix pd[gw*gh];Pix &ps(int x,int                        y){int i=x+y*gh;assert((i>0&&i<=gw*gh)
||"pxloutofrange");return pd[i];}void p(int x,int y,Pix                        &pn)                               {ps
(x,y)=pn;}void phl(int x,int y,int l,Pix &pn){Pix *p=&ps                       (x,   y);for(int   i=x;i<x         +l;
++i){if(i<0);else if(i>gw)return;else*p=pn;++p;}}void pvl(int                  x,int y,int l,      Pix &pn)       {Pix
*p=&ps(x,y);for (int i=y;i<y+l;++i){if(i<0);else if (i>gh)                     return;else*p=pn;   p+=gh;}}       void
pr(const SDL_Rect &r, Pix &pn, bool f=0){phl(r.x,r.y,r.w,   pn);phl(r.x,r      .y+r.                              h-1,r.
w,pn);pvl(r.x,r.y+1,r.h-2,pn);pvl(r.x+r.w-1,r.y+1,r.h-2,                pn);if (f)for                             (int i
=r.y+1;i<r.y+r.h-1;++i)phl(r.x+1,i,r.w-2,pn);}void specimen_guy                (int x,  int y,int m){pr           ({x,y,
 10, 10}, pw);p(x+2, y+2, pw);p(x+4, y+2, pw);pr({x+2,       y+4,6,m+1},pw,1); }template <typename T>T abs        (T v){
return v > 0 ? v : v < 0 ? -v : 0; }constexpr int pgl=6;                        struct Bruh{int ydiff;float x     ;float
 hsp;float acc;void uhm(){x = -3 - std::rand() %                               6;ydiff = -2 + std::rand() % 4;    hsp = 0;
}Bruh() : acc(0.05){uhm();}void bruh(){hsp +=                   acc;x += hsp;if(x > 43)uhm(); pr({(int) x,30+     ydiff,2,
2},cb);}};Bruh pg[pgl];int main(){double                tik=0;double            tk=0;SDL_Init(SDL_INIT_VIDEO);    SDL_Window
*w = SDL_CreateWindow("food",SDL_WINDOWPOS_UNDEFINED                           ,SDL_WINDOWPOS_UNDEFINED,          ww,wh,
                                                                                SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
                                                                                SDL_Renderer *r =SDL_CreateRenderer
                                (w, -1, SDL_RENDERER_PRESENTVSYNC);             SDL_Texture *t = SDL_CreateTexture(r,
                       SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_STREAMING,
                     gw,gh);SDL_Event e;bool c = 1;while (c){tik += 0.3;tk
                    += 0.1;while (SDL_PollEvent(&e) != 0){switch (e.type)
                  {case SDL_QUIT:c = 0;break;case SDL_KEYDOWN:if (e.key.
                 keysym.scancode==SDL_SCANCODE_P)c=0;break;}}Pix *pc=pd;
                 for (int i=0;i<gw*gh;++i){*pc=cg;++pc;}specimen_guy(38,
               24, std::sin(tik) * 3 + 3);for (auto &p:pg){p.bruh();}
           SDL_UpdateTexture(t,nullptr,(void*)pd,4*gw);SDL_RenderCopy(r,t,
              nullptr,nullptr);SDL_RenderPresent(r);}SDL_DestroyTexture(t)
                ;SDL_DestroyRenderer(r);SDL_DestroyWindow(w);
                    SDL_Quit();return 0;}

#if 0
#
#
#
#                                                        o
#                    oooooo                              o
#                  ooo           oooooo    oooooo     oooo
#                  oooooooooo   oo    o   oo    o    oo  o
#                  ooo          o     o  o      o   o    o
#                  ooo           ooooo   ooooooo    oooooo
#
            aaaaaaaaaaaaaaa
    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
 aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
 aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa                                    aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa                                    a                               a
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa                                 a   aaaaaa     aaaaaaa          a
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa                                  a   a aaaa     aaaaa a          a
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa                                   a   aaaaaa     aaaaaaa          a
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa         aaaaaaaa                   a                               a
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa        aaaaaaaaaaaa       aaaaa     a                               a
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa       aaaaaaaaaaaaaaa           aa   a      aaaaaaaaaa               a
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa     aaaaaaaaaaaaaaaaa                a    aaaaaaaaaaaaaa             a
   aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa        aaaaaaaaaaaaaaaa    aaaaaaaaaaa  a   aaaaaaaaaaaaaaaa            a
          aaaaaaaaaaaaaaaaaaaaaaaaaaa        aaaaaaaaaaaaa                    a   aaaaaaaaaaaaaaaaa           a
                 aaaaaaaaaaaaaaa                aaaaaaa                aaaa   a   aaaaaaaaaaaaaaaaa           a
                                                              aaaaaaaaa       a    aaaaaaaaaaaaaaa            a
                                                                              a                               a
                                      aaaaaaaaaaaaaaaa                        aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
                               aaaaaaaaaaaaaaaaaaaaaaaaaaa
                            aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
                           aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
                          aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
                         aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
                           aaaaaaaaaaaaaaaaaaaaaaaaaaaa
                                 aaaaaaaaaaaaaaaaaaaaa
                                   aaaaaaaaaaaaaaaa
made on 6.19.23
#endif
