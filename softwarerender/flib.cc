#include <cstdio>
#include <cstdint>

#include <SDL2/SDL.h>

namespace flib
{
	typedef std::uint8_t	u8;
	typedef std::uint16_t	u16;
	typedef std::uint32_t	u32;
	typedef std::uint64_t	u64;
	typedef std::int8_t	i8;
	typedef std::int16_t	i16;
	typedef std::int32_t	i32;
	typedef std::int64_t	i64;

	constexpr int WIN_WIDTH = 100;
	constexpr int WIN_HEIGHT = 100;

	struct Pix{
		u8 r, g, b, a;
	};

	struct State{
		i64 gw = WIN_WIDTH;
		i64 gh = WIN_HEIGHT;
		i64 gm = WIN_WIDTH * WIN_HEIGHT;

		Pix *pd;
	} state;

	template <typename T>
	T clamp(T val, T min, T max)
	{
		return val < min ? min : (val > max ? max : val);
	}
}

int main(void)
{
	using namespace flib;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *win = SDL_CreateWindow("flib", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture *t = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, state.gw, state.gh);

	bool game_running = true;
	SDL_Event e;

	state.pd = new Pix[state.gm];
	
	u64 pos = 0;
	i64 mx, my;
	mx = my = -1;

	while (game_running)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				game_running = false;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.scancode == SDL_SCANCODE_Q)
					game_running = false;
				break;
			case SDL_MOUSEMOTION:
				mx = e.motion.x;
				my = e.motion.y;
				break;
			}
		}

		u8 rep = 10;
		u8 spread = 2;
		for (u8 j = 0; j < rep; ++j)
		{
			Pix g = {255, 0, 0, 0};
			state.pd[pos * spread] = g;
			if (++pos > state.gm / spread)
				pos = 0;
		}
		for (int i = 0; i < state.gm; ++i)
		{
			Pix &p = state.pd[i];
			if (p.r > 0)
				--p.r;
			if (p.g > 0)
				--p.g;
			if (p.b > 0)
				--p.b;
		}
		if (mx >= 0 && mx < state.gw && my >= 0 && my < state.gh)
		{
			state.pd[mx + my * state.gw] = {0, 255, 0};
		}

		SDL_UpdateTexture(t, nullptr, (void *) state.pd, 4 * state.gw);
		SDL_RenderCopy(ren, t, nullptr, nullptr);
		SDL_RenderPresent(ren);
	}

	delete[] state.pd;
	SDL_DestroyTexture(t);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
