#include "chip8.hpp"

int main(int argc, char *argv[])
{
	Display display;
	Chip8 *chip = new Chip8(display);

	return 0;
}
