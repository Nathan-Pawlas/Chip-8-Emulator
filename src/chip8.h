#pragma once
#include <iostream>

class chip8
{
public:
	chip8();
	~chip8();

	void initialize();

	void emulate();

	void load_game(std::string file_path);
public:
	unsigned short opcode;
	unsigned char memory[4096];
	unsigned char registers[16];
	unsigned short index;
	unsigned short pc;
	unsigned char gfx[64 * 32];
	unsigned short stack[16];
	unsigned short stack_pointer;
	unsigned char delay_timer;
	unsigned char sound_timer;
	unsigned char key[16];
};

/*
Memory Map
0x0000 - 0x01FF - Chip-8 Interpreter
0x0050 - 0x00A0 - Built in 4x5 pixel font set (0-F)
0x0F00 - 0x0FFF - Program ROM, Stack, Variables
*/