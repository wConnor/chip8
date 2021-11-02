#include "chip8.hpp"
#include <SDL_events.h>
#include <cstdint>

Chip8::Chip8(Display &display) {
	// intialises the arrays with 0
	std::fill(memory.begin(), memory.end(), 0x0);
	std::fill(V.begin(), V.end(), 0x0);
	std::fill(stack.begin(), stack.end(), 0x0);

	memory[pc] = 0xDF;
	memory[pc + 1] = 0xD2;

	std::srand(std::time(nullptr));

	// randomises registers for testing. the final register (V[0xF]),
	// must not be  randomised as it's used for special purposes.
	for (int i = 0; i != V.size() -1; ++i) {
		V[i] = std::rand() % UINT8_MAX;
	}

	std::uint16_t op_code = (memory[pc] << 8) | memory[pc + 1];

	switch (op_code & 0xF000u) {
	case 0x0000:
		switch (op_code & 0x00FFu) {
		case 0x00E0: // 00E0: Clears the screen.
			std::cout << std::hex << "0x" << op_code << ": CLEARING SCREEN." << std::endl;
			display.clear();
			break;

		case 0x00EE: // 00EE: Returns from a subroutine.
			--stack_ptr;
			pc = stack[stack_ptr];
			break;
		}
		break;

	case 0x1000: // 1NNN: Jumps to address NNN.
		pc = op_code & 0x0FFFu;
		std::cout << std::hex << "0x" << op_code << ": JUMPING TO ADDRESS '0x" << pc << "'." << std::endl;
		break;

	case 0x2000: // 2NNN: Calls subroutine at NNN.
		stack[stack_ptr] = pc;
		++stack_ptr;
		pc = op_code & 0x0FFFu;
		std::cout << std::hex << "0x" << op_code << ": CALLING SUBROUTINE AT '0x" << pc << "'." << std::endl;
		break;

	case 0x3000: { // 3XNN: Skips the next instruction if VX equals NN.
		std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
		std::uint8_t nn = (op_code & 0x00FFu);

		if (V[Vx] == nn) {
			std::cout << std::hex << "0x" << op_code << ": SKIPPING NEXT INSTRUCTION." << std::endl;
			pc += 2;
		}

		break;
	}

	case 0x4000: { // 4XNN: Skips the next instruction if VX does not equal NN.
		std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
		std::uint8_t nn = (op_code & 0x00FFu);

		if (V[Vx] != nn) {
			std::cout << std::hex << "0x" << op_code << ": SKIPPING NEXT INSTRUCTION." << std::endl;
			pc += 2;
		}

		break;
	}

	case 0x5000: { // 5XY0: Skips the next instruction if VX equals VY.
		std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
		std::uint8_t Vy = (op_code & 0x00F0u) >> 4;

		if (V[Vx] == V[Vy]) {
			std::cout << std::hex << "0x" << op_code << ": SKIPPING NEXT INSTRUCTION." << std::endl;
			pc += 2;
		}

		break;
	}

	case 0x6000: { // 6XNN: Sets VX to NN.
		std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
		std::uint8_t nn = (op_code & 0x00FFu);

		std::cout << std::hex << "0x" << op_code << ": SETTING VALUE OF REGISTER '0x" << +Vx << "' TO '" << nn << "." << std::endl;
		V[Vx] = nn;

		break;
	}

	case 0x7000: { // 7XNN: Adds NN to VX.
		std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
		std::uint8_t nn = (op_code & 0x00FFu);

		std::cout << std::hex << "0x" << op_code << ": ADDING '0x" << +nn << " (" << std::dec << +nn << ")' TO REGISTER '0x" << std::hex << +Vx << "'." << std::endl;
		V[Vx] += nn;

		break;
	}

	case 0x8000: {
		std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
		std::uint8_t Vy = (op_code & 0x00F0u) >> 4;
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
			std::cout << std::hex << "0x" << op_code << ": ADDING REGISTER '0x" << +Vy << "' (val '" << +V[Vy] << "') TO '0x" << +Vx << "' (val '" << +V[Vx] << "')." << std::endl;

			V[0xF] = V[Vx] + V[Vy] > UINT8_MAX;

			std::cout << std::hex << "0x" << op_code << ": CARRY FLAG (REGISTER 0xF): " << +V[0xF] << '.' << std::endl;
			V[Vx] += V[Vy];
			break;

		case 0x0005: // 8XY5: VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not..
			std::cout << std::hex << "0x" << op_code << ": SUBTRACTING REGISTER '0x" << +Vy << "' FROM '0x" << +Vx << "'." << std::endl;
			V[0xF] = V[Vx] > V[Vy];

			std::cout << std::hex << "0x" << op_code << ": BORROW FLAG (REGISTER 0xF): " << +V[0xF] << '.' << std::endl;
			V[Vx] -= V[Vy];
			break;

		case 0x0006: // 8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
			std::cout << std::hex << "0x" << op_code << ": STORING LSB OF '0x" << +Vx << "' INTO REGISTER '0xF'." << std::endl;
			V[0xF] = V[Vx] & 0b0000'0001;
			std::cout << std::hex << "0x" << op_code << ": STORED '" << +V[0xF] << "' IN REGISTER '0xF'." << std::endl;

			std::cout << std::hex << "0x" << op_code << ": BITSHIFTING REGISTER '0x" << +Vx << "' TO THE RIGHT BY 1." << std::endl;
			V[Vx] >>= 1;
			break;

		case 0x0007: // 8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not.
			std::cout << std::hex << "0x" << op_code << ": STORING VALUE OF '0x" << +V[Vy] << "' - '0x" << +V[Vx] << "' IN REGISTER '0x" << +V[Vx] << "'." << std::endl;

			V[0xF] = V[Vx] < V[Vy];
			std::cout << std::hex << "0x" << op_code << ": BORROW FLAG (REGISTER 0xF): " << +V[0xF] << '.' << std::endl;

			V[Vx] = V[Vy] - V[Vx];
			break;

		case 0x000E: // 8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
			std::cout << std::hex << "0x" << op_code << ": STORING MSB OF '0x" << +V[Vx] << "' INTO '0xF'." << std::endl;
			V[0xF] = (V[Vx] & 0b1000'0000) >> 7;
			std::cout << std::hex << "0x" << op_code << ": STORED '0x" << +V[0xF] << "' IN REGISTER '0xF'." << std::endl;

			std::cout << std::hex << "0x" << op_code << ": BITSHIFTING REGISTER '0x" << +Vx << "' TO THE LEFT BY 1." << std::endl;
			V[Vx] <<= 1;
			break;

		default:
			std::cerr << "Unsupported opcode." << std::endl;
			break;
		}
	}

	case 0x9000: { // 9XY0: Skips the next instruction if VX does not equal VY.
		std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
		std::uint8_t Vy = (op_code & 0x00F0u) >> 4;

		if (V[Vx] != V[Vy]) {
			std::cout << std::hex << "0x" << op_code << ": SKIPPING NEXT INSTRUCTION." << std::endl;
			pc += 2;
		}

		break;
	}

	case 0xA000: { // ANNN: Sets I to the address NNN.
		std::uint8_t nnn = (op_code & 0x0FFFu);
		std::cout << std::hex << "0x" << op_code << ": SETTING I TO '" << +nnn << "'." << std::endl;
		I = nnn;
		break;
	}

	case 0xB000: { // BNNN: Jumps to the address NNN plus V0.
		std::uint8_t nnn = (op_code & 0x0FFFu);
		pc = nnn + V[0];
		std::cout << std::hex << "0x" << op_code << ": JUMPING TO ADDRESS '" << +pc << "'." << std::endl;
		break;
	}

	case 0xC000: { // CXNN: Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
		std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
		std::uint8_t nn = (op_code & 0x00FFu);
		std::uint8_t number = std::rand() % UINT8_MAX;

		V[Vx] = nn & number;
		std::cout << std::hex << "0x" << op_code << ": SETTING REGISTER '0x" << +Vx << "' TO BITWISE AND OF '0x"
				  << +nn << " (" << std::dec << +nn << ")' AND '0x" << std::hex << +number << "(" << std::dec
				  << +number << ")'." << std::endl;
		break;
	}

	case 0xD000: { // DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value does not change after the execution of this instruction.
		std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
		std::uint8_t Vy = (op_code & 0x00F0u) >> 4;
		std::uint8_t N = (op_code & 0x000Fu);

		std::cout << std::hex << "0x" << op_code << ": DRAWING 8 PIXEL WIDE SPRITE AT '(" << std::dec << +Vx << ',' << +Vy << ")' WITH HEIGHT OF " << +N << "." << std::endl;
		display.draw(Vx, Vy, N);
		SDL_Delay(2000);
		break;
	}

	case 0xE000:
		switch (op_code & 0x00FF) {
		case 0x009E: { // EX9E: Skips the next instruction if the key stored in VX is pressed.
			std::uint8_t Vx = (op_code & 0x0F00u) >> 8;

			SDL_Event event;
			SDL_PollEvent(&event);

			if (keys[event.key.keysym.sym] == Vx) {
				std::cout << std::hex << "0x" << op_code << ": SKIPPING NEXT INSTRUCTION AS KEY '" << +Vx << "IS PRESSED." << std::endl;
				pc += 2;
			}

			break;
		}

		case 0x00A1: { // EXA1: Skips the next instruction if the key stored in VX is not pressed.
			std::uint8_t Vx = (op_code & 0x0F00u) >> 8;

			SDL_Event event;
			SDL_PollEvent(&event);

			if (keys[event.key.keysym.sym] != Vx) {
				std::cout << std::hex << "0x" << op_code << ": SKIPPING NEXT INSTRUCTION AS KEY '" << +Vx << "IS NOT PRESSED." << std::endl;
				pc += 2;
			}

			break;
		}
		default:
			std::cerr << "Unsupported opcode." << std::endl;
			break;
		}

		break;

	case 0xF000:
		switch (op_code & 0x00FFu) {
		case 0x0007: { // FX07: Sets VX to the value of the delay timer.
			std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
			std::cout << std::hex << "0x" << op_code << ": SETTING REGISTER '0x" << +Vx<<  "TO VALUE OF DELAY TIMER (" << +delay_timer << "'." << std::endl;

			break;
		}

		case 0x000A: // FX0A: A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event).

			break;

		case 0x0015: { // FX15: Sets the delay timer to VX.
			std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
			std::cout << std::hex << "0x" << op_code << ": SETTING DELAY TIMER TO VALUE OF '0x" << +Vx << " (0x" << +V[Vx] << ")'." << std::endl;
			delay_timer = V[Vx];

			break;
		}

		case 0x0018: { // FX18: Sets the sound timer to VX.
			std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
			std::cout << std::hex << "0x" << op_code << ": SETTING SOUND TIMER TO VALUE OF '0x" << +Vx << " (0x" << +V[Vx] << ")'." << std::endl;
			sound_timer = V[Vx];

			break;
		}

		case 0x001E: { // FX1E: Adds VX to I. VF is not affected.
			std::uint8_t Vx = (op_code & 0x0F00u) >> 8;
			std::cout << std::hex << "0x" << op_code << ": ADDING VALUE OF '0x" << +Vx << " (0x" << +V[Vx] << ")' TO I (" << +I << ")." << std::endl;			
			I += V[Vx];

			break;
		}

		case 0x0029: // FX29: Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.

			break;

		case 0x0033: // FX33: Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2.

			break;

		case 0x0055: // FX55: Stores V0 to VX (including VX) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
			std::cout << std::hex << "0x" << op_code << ": STORING REGISTER VALUES INTO MEMORY FROM '0x" << +I << "' TO '0x" << I + V.size() << "'." << std::endl;						

			for (int i = 0, it = I; i != V.size(); ++i, ++it)
				memory[it] = V[i];

			break;

		case 0x0065: // FX65: Fills V0 to VX (including VX) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
			std::cout << std::hex << "0x" << op_code << ": FILLING REGISTER WITH MEMORY VALUES FROM '0x" << +I << "' TO '0x" << I + V.size() << "'." << std::endl;						

			for (int i = 0, it = I; i != V.size(); ++i, ++it)
				V[i] = memory[it];

			break;

		default:
			std::cerr << "Unsupported opcode." << std::endl;
			break;
		}

		break;

	default:
		std::cerr << "Unsupported opcode." << std::endl;
		break;
	}
}

Chip8::~Chip8() {}
