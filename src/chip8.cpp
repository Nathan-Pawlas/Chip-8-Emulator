#include "chip8.h"


chip8::chip8()
{
}

chip8::~chip8()
{
}

void chip8::initialize()
{
	pc = 0x0200; //Where our program begins
	index = 0;
	stack_pointer = 0;
	sound_timer = 0;
	delay_timer = 0;
	opcode = 0;

	//Clear everything
	for (int i = 0; i < 16; i++)
	{
		stack[i] = 0;
		key_inputs[i] = 0;
		reg[i] = 0;
	}
	for (int i = 0; i < 4096; i++)
	{
		memory[i] = 0;
	}
	for (int i = 0; i < 80; i++)
	{
		memory[i] = fonts[i];
	}
	disp_clear();
}



void chip8::emulate()
{
	//ISA Found Here: https://en.wikipedia.org/wiki/CHIP-8#Graphics_and_sound //
	
	//Fetch
	opcode = (memory[pc] << 8) | memory[pc+1]; //Loading 2bytes from memory into opcode
	//00XX << 8 = XX00 | 00YY = XXYY
	
	unsigned short a = 0;

	//Decode
	switch (opcode & 0xF000) //Main Instruction determined by first nibble
	{
	case 0x0000:
		a = opcode & 0x00FF;
		switch (a)
		{
		case 0x00E0:
			disp_clear();
			draw = true;
			pc += 2;
			break;
		case 0x00EE: //Basically a return call, get address and decrement sp
			stack_pointer -= 1;
			pc = stack[stack_pointer];
			pc += 2;
			break;
		}
		break;
	case 0x1000: //Jump To
		pc = 0x0FFF & opcode;
		break;
	case 0x2000: //Jump To, but save return address on stack
		stack[stack_pointer] = pc;
		stack_pointer += 1;
		pc = 0x0FFF & opcode;
		break;
	case 0x3000: //X == NN
		if (reg[(0x0F00 & opcode) >> 8] == (0x00FF & opcode))
			pc += 4; //PC skips instruction by jumbing 4bytes since since intructions are 2bytes
		else
			pc += 2;
		break;
	case 0x4000: //inverse of 0x3000
		if (reg[(0x0F00 & opcode) >> 8] != (0x00FF & opcode))
			pc += 4; 
		else
			pc += 2;
		break;
	case 0x5000: //X == Y
		if (reg[(0x0F00 & opcode) >> 8] == reg[(0x00F0 & opcode) >> 4])
			pc += 4;
		else
			pc += 2;
		break;
	case 0x6000: //X = NN
		reg[(0x0F00 & opcode) >> 8] = (0x00FF & opcode);
		pc += 2;
		break;
	case 0x7000: //X += NN
		reg[(0x0F00 & opcode) >> 8] += (0x00FF & opcode);
		pc += 2;
		break;
	case 0x8000:
		a = opcode & 0x000F; //Instruction 8 depends on the last nibble: 0x8XYa
		switch (a)
		{
		case 0x0000: //X = Y
			reg[(0x0F00 & opcode) >> 8] = reg[(0x00F0 & opcode) >> 4];
			pc += 2;
			break;
		case 0x0001: //X = X | Y
			reg[(0x0F00 & opcode) >> 8] |= reg[(0x00F0 & opcode) >> 4];
			pc += 2;
			break;
		case 0x0002: // X = X & Y
			reg[(0x0F00 & opcode) >> 8] &= reg[(0x00F0 & opcode) >> 4];
			pc += 2;
			break;
		case 0x0003: // X = X ^ Y
			reg[(0x0F00 & opcode) >> 8] ^= reg[(0x00F0 & opcode) >> 4];
			pc += 2;
			break;
		case 0x0004:
		{	//X + Y with Overflow
			unsigned char x = reg[(opcode & 0x0F00) >> 8];
			unsigned char y = reg[(opcode & 0x00F0) >> 4];
			unsigned char result = x + y;
			if ((x > 0 && y > 0 && result <= 0) || (x < 0 && y < 0 && result >= 0))
				reg[0xF] = 1;
			else
				reg[0xF] = 0;
			reg[(opcode & 0x0F00) >> 8] = result;
			pc += 2;
			break;
		}
		case 0x0005: //Subtraction with underflow
			reg[(opcode & 0x0F00) >> 8] -= reg[(opcode & 0x00F0) >> 4];
			if (reg[(opcode & 0x0F00) >> 8] >= reg[(opcode & 0x00F0) >> 4])
				reg[0xF] = 0;
			else
				reg[0xF] = 1;
			pc += 2;
			break;
		case 0x0006: //Store least sig bit in Flag Reg, then bitshift left 1
			reg[0xF] = 0x0001 & reg[(0x0F00 & opcode) >> 8];
			reg[(0x0F00 & opcode) >> 8] >>= 1;
			pc += 2;
			break;
		case 0x0007:
			reg[(opcode & 0x0F00) >> 8] = reg[(opcode & 0x00F0) >> 4] - reg[(opcode & 0x0F00) >> 8];
			if (reg[(opcode & 0x00F0) >> 4] >= reg[(opcode & 0x0F00) >> 8])
				reg[0xF] = 0;
			else
				reg[0xF] = 1;
			pc += 1;
			break;
		case 0x000E: //Store most sig bit in flag reg, bitshift right 1
			reg[0xF] = 0x8 & reg[(0x0F00 & opcode) >> 8];
			reg[(0x0F00 & opcode) >> 8] <<= 1;
			pc += 2;
			break;
		}
		break;
	case 0x9000:
		if (reg[(0x0F00 & opcode) >> 8] != reg[(0x00F0 & opcode) >> 4])
			pc += 4;
		else
			pc += 2;
		break;
	case 0xA000:
		index = opcode & 0x0FFF;
		pc += 2;
		break;
	case 0xB000:
		pc = reg[0] + (opcode & 0x0FFF);
		break;
	case 0xC000:
		reg[(opcode & 0x0F00) >> 8] = (rand() % 250) & (opcode & 0x00FF);
		pc += 2;
		break;
	case 0xD000:
	{
		//Overall design aided by this blog post: https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#dxyn-display
		uint8_t Vx = (opcode & 0x0F00) >> 8;
		uint8_t Vy = (opcode & 0x00F0) >> 4;
		auto spriteHeight = opcode & 0x000F;

		reg[0xF] = 0;

		auto xPos = reg[Vx];
		auto yPos = reg[Vy];

		for (int row = 0; row < spriteHeight; row++)
		{
			uint8_t sprite_byte = memory[index + row];
			for (int col = 0; col < 8; col++)
			{
				if ((sprite_byte & (0x80 >> col)) != 0)
				{
					uint8_t px = xPos + col;
					uint8_t py = yPos + row;
					uint16_t pixel = px + py * RESOLUTION_WIDTH;
					if (gfx[pixel])
						reg[0x0F] = 1;
					gfx[pixel] ^= 1;
				}
			}
		}

		draw = true;
		pc += 2;
		break;
	}
	case 0xE000:
		a = opcode & 0x00FF;
		switch (a)
		{
		case 0x009E:
			if (key_inputs[reg[(opcode & 0x0F00) >> 8]] != 0)
				pc += 4;
			else
				pc += 2;
			break;
		case 0x00A1:
			if (key_inputs[reg[(opcode & 0x0F00) >> 8]] == 0)
				pc += 4;
			else
				pc += 2;
			break;
		}
		break;
	case 0xF000:
		a = opcode & 0x00FF;
		switch (a)
		{
		case 0x0007:
			reg[(opcode & 0x0F00) >> 8] = delay_timer;
			pc += 2;
			break;
		case 0x000A:
			for (int i = 0; i < 16; i++)
			{
				if (key_inputs[i] == 1)
				{
					pc += 2; // Only increment the PC if a key is pressed
				}
			}
			break; // Otherwise, stay on the same opcode until a key is pressed
		case 0x0015:
			delay_timer = reg[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x0018:
			sound_timer = reg[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x001E:
			if (index + reg[(opcode & 0x0F00) >> 8] > 0xFFF)
				reg[0xF] = 1;
			else
				reg[0xF] = 0;
			index += reg[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;
		case 0x0029:
			index = reg[(opcode & 0x0F00) >> 8] * 5;
			pc += 2;
			break;
		case 0x0033:
		{
			//Used the following article for my understanding: https://www.allaboutcircuits.com/technical-articles/fundamentals-of-binary-coded-decimal-bcd/
			uint16_t binary = reg[(opcode & 0x0F00) >> 8];
			memory[index] = binary / 100;
			memory[index + 1] = (binary / 10) % 10;
			memory[index + 2] = binary % 10;
			pc += 2;
			break;
		}
		case 0x0055:
		{
			int n = (opcode & 0x0F00) >> 8;
			for (int i = 0; i <= n; i++)
			{
				memory[index + i] = reg[i];
			}
			pc += 2;
			break;
		}
		case 0x0065:
		{
			int n = (opcode & 0x0F00) >> 8;
			for (int i = 0; i <= n; i++)
			{
				reg[i] = memory[index + i];
			}
			pc += 2;
			break;
		}
		}
		break;
	}
}

void chip8::load_game(const char* file_path)
{
	//File Reading Source: https://stackoverflow.com/questions/5420317/reading-and-writing-binary-file //

	std::ifstream input(file_path, std::ios::binary | std::ios::in);
	
	if (!input.is_open())
	{
		std::cerr << "ROM could not be loaded, ensure file path is correct!";
		exit(-1);
	}

	// copies all data into buffer
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
	
	input.close();


	for (int i = 0; i < buffer.size(); i++)
		memory[i + 0x200] = buffer.at(i);
}

void chip8::disp_clear()
{
	for (int i = 0; i < 64 * 32; i++)
	{
		gfx[i] = 0;
	}
}
