#include "display.hpp"
#include <SDL2/SDL_keycode.h>
#include <iostream>
#include <algorithm>
#include <map>
#include <ctime>
#include <random>
#include <climits>

constexpr std::uint16_t MEMORY_SIZE = 4096;
constexpr std::uint8_t STACK_SIZE = 16;
constexpr std::uint8_t REGISTER_COUNT = 16;

class Chip8
{
private:
	std::array <uint8_t, MEMORY_SIZE> memory;
	std::array <uint8_t, REGISTER_COUNT> V; /* data registers: V0 to VF */
	uint16_t I = 0x0; /* address register */
	uint16_t pc = 0x200;
	std::array <uint8_t, STACK_SIZE> stack;
	std::uint8_t stack_ptr = 0;
	std::uint8_t delay_timer = 0;
	std::uint8_t sound_timer = 0;
	std::map <int, int> keys = {
		{SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
		{SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
		{SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
		{SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF}
	};

public:
	Chip8(Display &display);
	virtual ~Chip8();
};
