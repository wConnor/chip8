#include "display.hpp"

Display::Display()
{
	std::fill(memory.begin(), memory.end(), 0x0);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
	}

	win = SDL_CreateWindow("CHIP-8", 100, 100, SCREEN_WIDTH * 10, SCREEN_HEIGHT * 10, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
	}

	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (texture == nullptr) {
		std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
	}

	// example pixels to fill
	memory[64] = 1;
	memory[2] = 1;
	memory[3] = 1;
	memory[65] = 1;
	update();
	SDL_Delay(2000);

	std::cout << "Clearing display memory..." << std::endl;
	clear();
	update();
	SDL_Delay(2000);
}

Display::~Display()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void Display::update()
{
	std::uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
	for (int i = 0; i != memory.size(); ++i) {
		if (memory[i] == 0)  {
			pixels[i] = 0xFF000000;
		} else if (memory[i] == 1) {
			pixels[i] = 0xFFFFFFFF;
		}
	}

	SDL_UpdateTexture(texture, nullptr, pixels, SCREEN_WIDTH * sizeof(pixels[0]));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}

void Display::clear()
{
	std::fill(memory.begin(), memory.end(), 0x0);
}
