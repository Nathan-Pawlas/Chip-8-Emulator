#pragma once
// Class includes
#include "chip8.h"

// SFML includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// STL includes
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

#define SCREEN_WIDTH 768
#define SCREEN_HEIGHT 384

class Driver
{
public:
	Driver()
	{};
	~Driver()
	{};

	void run(chip8* cpu)
	{

		sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Chip8");
		backgroundColor = sf::Color(35,35,35,255);
		chip = cpu;
		pixel.setSize(sf::Vector2f(pixelscale, pixelscale));
		pixel.setFillColor(sf::Color(200,200,200,255));

		clear_screen();

		sf::Clock clock;
		sf::Time time = clock.getElapsedTime();

		uint32_t total_cycles = 0;

		while (window.isOpen())
		{
			chip->emulate();

			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) {
					window.close();
				}
				if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
					handle_inputs(event, cpu);
			}
			
			update_timers();

			if (chip->draw) {
				chip->draw = false;
				window.clear(backgroundColor);
				for (int y = 0; y < 32; y++)
				{
					for (int x = 0; x < 64; x++)
					{
						if (chip->gfx[y * 64 + x])
						{
							pixel.setPosition(x * pixelscale, y * pixelscale);
							window.draw(pixel);
						}
					}
				}

				window.display();

				chip->reg[0xF] = 0;
			}			
			std::this_thread::sleep_for(std::chrono::microseconds(6)); //Delay the processing speed
		}
	}

public:
	unsigned char display[64 * 32];

private:
	void handle_inputs(sf::Event event, chip8* cpu)
	{
		
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Num1)
				cpu->key_inputs[0x1] = 1;
			else if (event.key.code == sf::Keyboard::Num2)
				cpu->key_inputs[0x2] = 1;
			else if (event.key.code == sf::Keyboard::Num3)
				cpu->key_inputs[0x3] = 1;
			else if (event.key.code == sf::Keyboard::Num4)
				cpu->key_inputs[0xC] = 1;
			else if (event.key.code == sf::Keyboard::Q)
				cpu->key_inputs[0x4] = 1;
			else if (event.key.code == sf::Keyboard::W)
				cpu->key_inputs[0x5] = 1;
			else if (event.key.code == sf::Keyboard::E)
				cpu->key_inputs[0x6] = 1;
			else if (event.key.code == sf::Keyboard::R)
				cpu->key_inputs[0xD] = 1;
			else if (event.key.code == sf::Keyboard::A)
				cpu->key_inputs[0x7] = 1;
			else if (event.key.code == sf::Keyboard::S)
				cpu->key_inputs[0x8] = 1;
			else if (event.key.code == sf::Keyboard::D)
				cpu->key_inputs[0x9] = 1;
			else if (event.key.code == sf::Keyboard::F)
				cpu->key_inputs[0xE] = 1;
			else if (event.key.code == sf::Keyboard::Z)
				cpu->key_inputs[0xA] = 1;
			else if (event.key.code == sf::Keyboard::X)
				cpu->key_inputs[0x0] = 1;
			else if (event.key.code == sf::Keyboard::C)
				cpu->key_inputs[0xB] = 1;
			else if (event.key.code == sf::Keyboard::V)
				cpu->key_inputs[0xF] = 1;
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Num1)
				cpu->key_inputs[0x1] = 0;
			else if (event.key.code == sf::Keyboard::Num2)
				cpu->key_inputs[0x2] = 0;
			else if (event.key.code == sf::Keyboard::Num3)
				cpu->key_inputs[0x3] = 0;
			else if (event.key.code == sf::Keyboard::Num4)
				cpu->key_inputs[0xC] = 0;
			else if (event.key.code == sf::Keyboard::Q)
				cpu->key_inputs[0x4] = 0;
			else if (event.key.code == sf::Keyboard::W)
				cpu->key_inputs[0x5] = 0;
			else if (event.key.code == sf::Keyboard::E)
				cpu->key_inputs[0x6] = 0;
			else if (event.key.code == sf::Keyboard::R)
				cpu->key_inputs[0xD] = 0;
			else if (event.key.code == sf::Keyboard::A)
				cpu->key_inputs[0x7] = 0;
			else if (event.key.code == sf::Keyboard::S)
				cpu->key_inputs[0x8] = 0;
			else if (event.key.code == sf::Keyboard::D)
				cpu->key_inputs[0x9] = 0;
			else if (event.key.code == sf::Keyboard::F)
				cpu->key_inputs[0xE] = 0;
			else if (event.key.code == sf::Keyboard::Z)
				cpu->key_inputs[0xA] = 0;
			else if (event.key.code == sf::Keyboard::X)
				cpu->key_inputs[0x0] = 0;
			else if (event.key.code == sf::Keyboard::C)
				cpu->key_inputs[0xB] = 0;
			else if (event.key.code == sf::Keyboard::V)
				cpu->key_inputs[0xF] = 0;
		}
	}

	void clear_screen()
	{
		for (int i = 0; i < 64 * 32; i++)
		{
			chip->gfx[i] = 0;
		}
	}

	void update_timers()
	{
		if (chip->delay_timer > 0)
			chip->delay_timer -= 1;
		if (chip->sound_timer > 0)
			chip->sound_timer -= 1;
	}


private:
	chip8* chip;

	float_t pixelscale = 12.0f;

	sf::RenderWindow window;
	sf::Color backgroundColor;
	sf::RectangleShape pixel;
};


/*
Fonts

0:					1:					2:					3:
##########			##########			##########			##########
#01100000# 0x60		#11110000# 0xF0		#11110000# 0xF0		#11110000# 0xF0
#11010000# 0xB0		#00110000# 0x30		#10010000# 0x90		#00010000# 0x10
#11010000# 0xB0		#00110000# 0x30		#00100000# 0x20		#01110000# 0x70
#11010000# 0xB0		#00110000# 0x30		#01000000# 0x40		#00010000# 0x10
#01100000# 0x60		#01111000# 0x78		#11110000# 0xF0		#11110000# 0xF0
##########			##########			##########			##########

4:					5:					6:					7:
##########			##########			##########			##########
#10010000# 0x90		#11110000# 0xF0		#11110000# 0xF0		#11110000# 0xF0
#10010000# 0x90		#10000000# 0x80		#10000000# 0x80		#10010000# 0x90
#11110000# 0xF0		#11110000# 0xF0		#11110000# 0xF0		#00111000# 0x38
#00010000# 0x10		#00010000# 0x10		#10010000# 0x90		#00010000# 0x10
#00010000# 0x10		#11110000# 0xF0		#11110000# 0xF0		#00010000# 0x10
##########			##########			##########			##########

8:					9:					A:					B:
##########			##########			##########			##########
#11110000# 0xF0		#11110000# 0xF0		#01100000# 0x60		#11100000# 0xE0
#10010000# 0x90		#10010000# 0x90		#10010000# 0x90		#10010000# 0x90
#11110000# 0xF0		#11110000# 0xF0		#11110000# 0xF0		#11100000# 0xE0
#10010000# 0x90		#00010000# 0x10		#10010000# 0x90		#10010000# 0x90
#11110000# 0xF0		#00010000# 0x10		#10010000# 0x90		#11100000# 0xE0
##########			##########			##########			##########

C:					D:					E:					F:
##########			##########			##########			##########
#11110000# 0xF0		#11100000# 0xE0		#11110000# 0xF0		#11110000# 0xF0
#10000000# 0x80		#10010000# 0x90		#10000000# 0x80		#10000000# 0x80
#10000000# 0x80		#10010000# 0x90		#11110000# 0xF0		#11110000# 0xF0
#10000000# 0x80		#10010000# 0x90		#10000000# 0x80		#10000000# 0x80
#11110000# 0xF0		#11100000# 0xE0		#11110000# 0xF0		#10000000# 0x80
##########			##########			##########			##########
*/