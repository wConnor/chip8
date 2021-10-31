#include <iostream>
#include <algorithm>
#include <array>

constexpr std::uint16_t MEMORY_SIZE = 4096;
constexpr std::uint8_t STACK_SIZE = 16;
constexpr std::uint8_t REGISTER_COUNT = 16;
constexpr std::uint8_t KEY_COUNT = 16;
constexpr std::uint16_t DISPLAY_SIZE = 16 * 32;

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
	std::array <uint8_t, KEY_COUNT> keys;
	std::array <uint8_t, DISPLAY_SIZE> display;

public:
	Chip8();
	virtual ~Chip8();
};
