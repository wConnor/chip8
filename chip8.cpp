#include "chip8.hpp"

Chip8::Chip8(Display &display) {
	/* intialises the arrays with 0 */
	std::fill(memory.begin(), memory.end(), 0x0);
	std::fill(V.begin(), V.end(), 0x0);
	std::fill(stack.begin(), stack.end(), 0x0);

	memory[pc] = 0x85;
	memory[pc + 1] = 0x20;

	std::srand(std::time(nullptr));

	// memory randomiser. the final register (V[0xF]), must not be
	// randomised as it's used for carry + borrow flags.
	for (int i = 0; i != V.size() -1; ++i) {
		V[i] = std::rand() % 8;
	}

	std::uint16_t op_code = (memory[pc] << 8) | memory[pc + 1];
	std::cout << "op_code: " << std::hex << op_code << std::endl;

	switch (op_code & 0xF000) {
	case 0x1000:

		break;
	case 0x2000:

		break;
	case 0x3000:

		break;
	case 0x4000:


		break;
	case 0x5000:


		break;
	case 0x6000:

		break;
	case 0x7000:

		break;
	case 0x8000: {
		std::uint8_t Vx = (op_code & 0x0F00u) >> 8u;
		std::uint8_t Vy = (op_code & 0x00F0u) >> 4u;
		switch (op_code & 0x000F) {
		case 0x0000: // 8XY0: Sets VX to the value of VY.
			std::cout << std::hex << "0x" << op_code << ": SETTING REGISTER '0x" << +Vx << "' TO VALUE OF '0x" << +Vy << "'." << std::endl;
			V[Vx] = V[Vy];
			break;

		case 0x0001: // 8XY1: Sets VX to VX or VY. (Bitwise OR operation).
			std::cout << std::hex << "0x" << op_code << ": SETTING REGISTER '0x" << +Vx << "' TO VALUE OF '0x" << +Vx << "' OR '" << +Vy << "'." << std::endl;
			V[Vx] |= V[Vy];
			break;

		case 0x0002: // 8XY2: Sets VX to VX and VY. (Bitwise AND operation).
			std::cout << std::hex << "0x" << op_code << ": SETTING REGISTER '0x" << +Vx << "' TO VALUE OF '0x" << +Vx << "' AND '" << +Vy << "'." << std::endl;
			V[Vx] &= V[Vy];
			break;

		case 0x0003: // 8XY3: Sets VX to VX xor VY. (Bitwise XOR operation).
			std::cout << std::hex << "0x" << op_code << ": SETTING REGISTER '0x" << +Vx << "' TO VALUE OF '0x" << +Vx << "' XOR '" << +Vy << "'." << std::endl;
			V[Vx] ^= V[Vy];
			break;

		case 0x0004: // 8XY4: Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not.
			std::cout << std::hex << "0x" << op_code << ": ADDING REGISTER '0x" << +Vy << "' TO '0x" << +Vx << "'." << std::endl;
			if (V[Vx] + V[Vy] > 255u)
				V[0xF] = 1;
			else
				V[0xF] = 0;

			V[Vx] += V[Vy];
			break;

		case 0x0005: // 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not..
			std::cout << std::hex << "0x" << op_code << ": SUBTRACTING REGISTER '0x" << +Vy << "' FROM '0x" << +Vx << "'." << std::endl;
			if (V[Vx] > V[Vy])
				V[0xF] = 1;
			else
				V[0xF] = 0;

			V[Vx] -= V[Vy];
			break;

		case 0x0006: // 8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1.

			break;
		}
	}

		break;
	case 0x9000:

		break;
	case 0xA000:

		break;
	case 0xB000:

		break;
	case 0xC000:

		break;
	case 0xD000:

		break;
	case 0xE000:

		break;
	case 0xF000:

		break;

	default:
		std::cerr << "Unsupported opcode." << std::endl;
		break;
	}

}

Chip8::~Chip8() {}
