#include <iostream>
#include <algorithm>
#include <array>

class Chip8
{
private:
	std::array <char, 4096> memory;
	std::array <char, 16> v_regs;
	char i_ptr = 0x0;
	int pc = 0x200;
	std::array <char, 16> stack;
	int stack_ptr = 0;
	int delay_timer = 0;
	int sound_timer = 0;
	std::array <char, 16> keys;
	std::array <char, 16 * 32> display;

public:
	Chip8();
	virtual ~Chip8();
};
