#include <SDL2/SDL.h>
#include <array>
#include <iostream>

constexpr std::uint16_t SCREEN_WIDTH = 64 * 10;
constexpr std::uint16_t SCREEN_HEIGHT = 32 * 10;
constexpr std::uint8_t PIXEL_SIZE = 8 * 10;

class Display
{
private:
	std::array <uint8_t, SCREEN_WIDTH * SCREEN_HEIGHT> memory;
	SDL_Window *win;
	SDL_Renderer *renderer;
	SDL_Surface *surface;

public:
	Display();
	virtual ~Display();
	void update();
	void clear();
	std::array<uint8_t, SCREEN_WIDTH * SCREEN_HEIGHT> get_memory();
};
