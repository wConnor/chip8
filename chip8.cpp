#include "chip8.hpp"

Chip8::Chip8(Display &display) {
	/* intialises the arrays with 0 */
	std::fill(memory.begin(), memory.end(), 0x0);
	std::fill(V.begin(), V.end(), 0x0);
	std::fill(stack.begin(), stack.end(), 0x0);
	std::fill(keys.begin(), keys.end(), 0x0);

	std::uint16_t op_code = (memory[pc] << 8) | memory[pc + 1];
	std::cout << "op_code: " << std::hex << op_code << std::endl;
}

Chip8::~Chip8() {}
