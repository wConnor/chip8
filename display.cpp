#include "display.hpp"
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include <SDL_surface.h>

Display::Display()
{
	std::fill(memory.begin(), memory.end(), 0x0);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
	}

	win = SDL_CreateWindow("CHIP-8", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
	}

	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH , SCREEN_HEIGHT, 32, 0, 0, 0, 0);
	update();
	SDL_Delay(5000);
}

Display::~Display()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void Display::update()
{
	for (int i = 0; i != memory.size(); ++i) {
		if (memory[i] == 0)  {
			// black pixel
		} else if (memory[i] == 1) {
			// white pixel
		}
	}
	SDL_RenderPresent(renderer);
}

void Display::clear()
{
	std::fill(memory.begin(), memory.end(), 0x0);
}

std::array<uint8_t, SCREEN_WIDTH * SCREEN_HEIGHT> Display::get_memory()
{
	return memory;
}
