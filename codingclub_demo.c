// By Luke L
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Comment to disable vsync
#define	VSYNC

// If vsync is disabled, this value represents how many ticks the program will wait between each frame
#define	FRAME_TICKS	30

#define	TEXT_YSPACE	100

#define	SRC_READ_MAX	10000

typedef struct{
	SDL_Texture *tex;
	int w;
	int h;
} CCDemoText;

TTF_Font *g_font;
SDL_Window *g_window;
SDL_Renderer *g_renderer;

void ccdemo_hsv_to_rgb(double h, double s, double v, unsigned int *pr, unsigned int *pg, unsigned int *pb);
void ccdemo_draw_text(CCDemoText *text, int x, int y, float scale, float rot);
CCDemoText *ccdemo_load_text(char *str);
CCDemoText *ccdemo_load_text_wrap(char *str, int wrap);
char *get_src_code(void);

int main(int argc, char **argv)
{
	int screen_width = 640;
	int screen_height = 480;

	// Initialize things
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "failed to initialize SDL. SDL Error: %s\n", SDL_GetError());
		return 1;
	}
	if ((g_window = SDL_CreateWindow("Coding Club SDL Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) == NULL)
	{
		fprintf(stderr, "failed to create window. SDL Error: %s\n", SDL_GetError());
		return 1;
	}
#ifdef	VSYNC
	int renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
#else
	int renderer_flags = SDL_RENDERER_ACCELERATED;
#endif
	if ((g_renderer = SDL_CreateRenderer(g_window, -1, renderer_flags)) == NULL)
	{
		fprintf(stderr, "failed to create renderer. SDL Error: %s\n", SDL_GetError());
		return 1;
	}
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "failed to initialize SDL_ttf. SDL Error: %s\n", TTF_GetError());
		return 1;
	}
	if ((g_font = TTF_OpenFont("freemono.ttf", 17)) == NULL)
	{
		fprintf(stderr, "failed to open font. SDL Error: %s\n", TTF_GetError());
		return 1;
	}
	// Demo loop
	bool demo_running = true;

	// Background color values
	unsigned int cr, cg, cb, hue;
	hue = 0;
	
	CCDemoText *t_title, *t_tag, *t_yes, *t_code;
	t_title = ccdemo_load_text("CODING & WEB DESIGN CLUB");
	t_tag = ccdemo_load_text("We code things");
	t_yes = ccdemo_load_text("because yes.");
	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);
	t_code = ccdemo_load_text_wrap(get_src_code(), dm.w);

	SDL_Event e;

	uint32_t tick_this_frame, tick_last_frame;

	while (demo_running)
	{
		// Set frame start ticks
		tick_this_frame = SDL_GetTicks();
		tick_last_frame = tick_this_frame;

		// Handle SDL events
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			switch (e.type)
			{
			case SDL_QUIT:
				demo_running = false;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_q:
					demo_running = false;
					break;
				}
				break;
			case SDL_WINDOWEVENT:
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					screen_width = e.window.data1;
					screen_height = e.window.data2;
					SDL_RenderPresent(g_renderer);
					break;
				case SDL_WINDOWEVENT_EXPOSED:
					SDL_RenderPresent(g_renderer);
					break;
				}
				break;
			}
		}

		// Clear screen with rainbow
		if (++hue > 360)
			hue = 0;
		ccdemo_hsv_to_rgb(hue, 1, 1, &cr, &cg, &cb);
		SDL_SetRenderDrawColor(g_renderer, cr, cg, cb, 255);
		SDL_RenderClear(g_renderer);

		// Render text
		static float rot = 0;
		static float scale = 1;
		static float sinx = 0;
		rot += 1;
		scale += 0.03;
		sinx += 0.02;
		scale = sinf(sinx) + 2; 
		ccdemo_draw_text(t_title, screen_width/2, screen_height/2 - TEXT_YSPACE, scale + 1, sinf(sinx) * 40);
		ccdemo_draw_text(t_tag, screen_width/2, screen_height/2 + TEXT_YSPACE/2, scale, rot);
		ccdemo_draw_text(t_yes, screen_width/2, screen_height/2 + TEXT_YSPACE*2, scale, rot);

		// Draw source code until it fills the screen
		for (int ydrawn = 0; ydrawn < screen_height;)
		{
			ccdemo_draw_text(t_code, t_code->w / 2, ydrawn + t_code->h / 2, 1, 0);
			ydrawn += t_code->h;
		}

		// Render what's currently on the screen
		SDL_RenderPresent(g_renderer);

	#ifndef	VSYNC
		// Manually delay the program if VSYNC is disabled
		uint32_t tick_diff = SDL_GetTicks() - tick_this_frame;
		if (tick_diff < FRAME_TICKS)
			SDL_Delay(FRAME_TICKS - tick_diff);
	#endif
	}
	TTF_CloseFont(g_font);
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
void ccdemo_hsv_to_rgb(double h, double s, double v, unsigned int *pr, unsigned int *pg, unsigned int *pb)
{
	double r, g, b;
	if (s == 0)
	{
		r = v;
		g = v;
		b = v;
	}
	else
	{
		double f, p, q, t;
		h = h == 360 ? 0 : h / 60;
		int i = (int) h;
		f = h - i;
		p = v * (1 - s);
		q = v * (1 - (s * f));
		t = v * (1 - (s * (1 - f)));
		switch (i)
		{
		case 0:
			r = v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = v;
			b = p;
			break;
		case 2:
			r = p;
			g = v;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = v;
			break;
		case 4:
			r = t;
			g = p;
			b = v;
			break;
		default:
			r = v;
			g = p;
			b = q;
			break;
		}
	}
	*pr = r * 255;
	*pg = g * 255;
	*pb = b * 255;
}
void ccdemo_draw_text(CCDemoText *text, int x, int y, float scale, float rot)
{
	int w = text->w * scale;
	int h = text->h * scale;
	SDL_Rect drect = {x-w/2, y-h/2, w, h};
	SDL_RenderCopyEx(g_renderer, text->tex, NULL, &drect, rot, NULL, SDL_FLIP_NONE);
}
CCDemoText *ccdemo_load_text(char *str)
{
	CCDemoText *text = malloc(sizeof(CCDemoText));

	SDL_Color col = {255, 255, 255, 255};
	SDL_Surface *surf;
	if ((surf = TTF_RenderText_Solid(g_font, str, col)) == NULL)
	{
		return NULL;
	}
	if ((text->tex = SDL_CreateTextureFromSurface(g_renderer, surf)) == NULL)
	{
		SDL_FreeSurface(surf);
		free(text);
		return NULL;
	}
	// Setting text dimensions
	text->w = surf->w;
	text->h = surf->h;
	SDL_FreeSurface(surf);
	return text;
}
CCDemoText *ccdemo_load_text_wrap(char *str, int wrap)
{
	CCDemoText *text = malloc(sizeof(CCDemoText));

	SDL_Color col = {255, 255, 255, 255};
	SDL_Surface *surf;
	if ((surf = TTF_RenderText_Blended_Wrapped(g_font, str, col, wrap)) == NULL)
	{
		return NULL;
	}
	if ((text->tex = SDL_CreateTextureFromSurface(g_renderer, surf)) == NULL)
	{
		SDL_FreeSurface(surf);
		free(text);
		return NULL;
	}
	// Setting text dimensions
	text->w = surf->w;
	text->h = surf->h;
	SDL_FreeSurface(surf);
	return text;
}
char *get_src_code(void)
{
	char *str = calloc(SRC_READ_MAX, sizeof(char));
	FILE *fp;
	if ((fp = fopen("codingclub_demo.c", "r")) == NULL)
	{
		fprintf(stderr, "error opening file \"codingclub_demo.\"\n");
		return NULL;
	}
	int c;
	int i = 0;
	str[0] = '\0';
	while ((c = fgetc(fp)) != EOF)
	{
		if (c == '\n' || c == '\t')
			continue;
		else
			str[i] = c;
		if (++i == SRC_READ_MAX - 1)
			break;
	}
	str[i] = '\0';
	fclose(fp);
	return str;
}
