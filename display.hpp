#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <array>
#include <iostream>

constexpr std::uint16_t WINDOW_WIDTH = 640;
constexpr std::uint16_t WINDOW_HEIGHT = 320;
constexpr std::uint16_t SCREEN_WIDTH = 64;
constexpr std::uint16_t SCREEN_HEIGHT = 32;

class Display
{
private:
	std::array <uint8_t, SCREEN_WIDTH * SCREEN_HEIGHT> memory;
	SDL_Window *win;
	SDL_Renderer *renderer;
	SDL_Surface *surface;
	SDL_Texture *texture;

public:
	Display();
	virtual ~Display();
	void update();
	void clear();
};
