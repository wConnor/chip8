#include "chip8.hpp"

Chip8::Chip8() {
	/* intialises the arrays with 0 */
	std::fill(memory.begin(), memory.end(), 0x30);
	std::fill(v_regs.begin(), v_regs.end(), 0x30);
	std::fill(stack.begin(), stack.end(), 0x30);
	std::fill(keys.begin(), keys.end(), 0x30);
	std::fill(display.begin(), display.end(), 0x30);

	char op_code = (memory[pc] << 8) | memory[pc + 1];
	std::cout << "op_code: " << std::hex << op_code << std::endl;
}

Chip8::~Chip8() {}
