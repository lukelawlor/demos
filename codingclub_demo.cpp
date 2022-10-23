// By Luke L
#include <iostream>

#include <cstdint>
#include <cstdlib>

#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Comment to disable vsync
#define	VSYNC

// If vsync is disabled, this value represents how many ticks the program will wait between each frame
namespace constants
{
	inline constexpr auto FRAME_TICKS {30};
	inline constexpr auto TEXT_YSPACE {100};
	inline constexpr auto SRC_READ_MAX {10000};
}

struct CCDemoText
{
	SDL_Texture *tex;
	int w;
	int h;
};

static TTF_Font *g_font;
static SDL_Window *g_window;
static SDL_Renderer *g_renderer;

void ccdemo_hsv_to_rgb(double h, double s, double v, unsigned int *pr, unsigned int *pg, unsigned int *pb);
void ccdemo_draw_text(CCDemoText *text, int x, int y, float scale, float rot);
CCDemoText *ccdemo_load_text(const char *str);
CCDemoText *ccdemo_load_text_wrap(const char *str, int wrap);
const char *get_src_code(void);

int main(int argc, char **argv)
{
	auto screen_width {640};
	auto screen_height {480};

	// Initialize things
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "failed to initialize SDL. SDL Error: " << SDL_GetError() << '\n';
		return 1;
	}
	if ((g_window = SDL_CreateWindow("Coding Club SDL Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) == NULL)
	{
		std::cerr << "failed create window. SDL Error: " << SDL_GetError() << '\n';
		return 1;
	}
#ifdef	VSYNC
	int renderer_flags {SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};
#else
	int renderer_flags {SDL_RENDERER_ACCELERATED};
#endif
	if ((g_renderer = SDL_CreateRenderer(g_window, -1, renderer_flags)) == NULL)
	{
		std::cerr << "failed create renderer. SDL Error: " << SDL_GetError() << '\n';
		return 1;
	}
	if (TTF_Init() == -1)
	{
		std::cerr << "failed initialize SDL_ttf. SDL Error: " << SDL_GetError() << '\n';
		return 1;
	}
	if ((g_font = TTF_OpenFont("freemono.ttf", 17)) == NULL)
	{
		std::cerr << "failed to open font. SDL Error: " << SDL_GetError() << '\n';
		return 1;
	}
	// Demo loop
	bool demo_running {true};

	// Background color values
	unsigned cr, cg, cb;
	double hue {0};
	
	CCDemoText *t_title, *t_tag, *t_yes, *t_code;
	t_title = ccdemo_load_text("CODING & WEB DESIGN CLUB");
	t_tag = ccdemo_load_text("We code things");
	t_yes = ccdemo_load_text("because yes.");
	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);
	t_code = ccdemo_load_text_wrap(get_src_code(), dm.w);

	if (t_title == NULL || t_tag == NULL || t_yes == NULL || t_code == NULL)
	{
		std::cerr << "failed to load text texture\n";
		return 1;
	}

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
		static float rot {0};
		static float scale {1};
		static float sinx {0};
		rot += 1;
		scale += 0.03;
		sinx += 0.02;
		scale = sinf(sinx) + 2; 
		ccdemo_draw_text(t_title, screen_width/2, screen_height/2 - constants::TEXT_YSPACE, scale + 1, sinf(sinx) * 40);
		ccdemo_draw_text(t_tag, screen_width/2, screen_height/2 + constants::TEXT_YSPACE/2, scale, rot);
		ccdemo_draw_text(t_yes, screen_width/2, screen_height/2 + constants::TEXT_YSPACE*2, scale, rot);

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
		if (tick_diff < constants::FRAME_TICKS)
			SDL_Delay(constants::FRAME_TICKS - tick_diff);
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
		h = {h == 360 ? 0 : h / 60};
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
	auto w = static_cast<int>(text->w * scale);
	auto h = static_cast<int>(text->h * scale);
	SDL_Rect drect = {static_cast<int>(x-w/2), static_cast<int>(y-h/2), w, h};
	SDL_RenderCopyEx(g_renderer, text->tex, NULL, &drect, rot, NULL, SDL_FLIP_NONE);
}
CCDemoText *ccdemo_load_text(const char *str)
{
	CCDemoText *text {static_cast<CCDemoText *>(std::malloc(sizeof(CCDemoText)))};

	SDL_Color col {255, 255, 255, 255};
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
CCDemoText *ccdemo_load_text_wrap(const char *str, int wrap)
{
	CCDemoText *text {static_cast<CCDemoText *>(std::malloc(sizeof(CCDemoText)))};

	SDL_Color col {255, 255, 255, 255};
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
const char *get_src_code(void)
{
	char *str {static_cast<char *>(std::calloc(constants::SRC_READ_MAX, sizeof(char)))};
	std::FILE *fp;
	if ((fp = std::fopen("codingclub_demo.cpp", "r")) == NULL)
	{
		std::cerr << "error opening file \"codingclub_demo.cpp\"\n";
		return NULL;
	}
	int c;
	int i {0};
	str[0] = '\0';
	while ((c = fgetc(fp)) != EOF)
	{
		if (c == '\n' || c == '\t')
			continue;
		else
			str[i] = c;
		if (++i == constants::SRC_READ_MAX - 1)
			break;
	}
	str[i] = '\0';
	fclose(fp);
	return static_cast<const char *>(str);
}
