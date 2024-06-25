#pragma once
// Class includes
#include "chip8.h"

// SFML includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// STL includes
#include <iostream>
#include <fstream>
#include <ctime>
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


		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) {
					window.close();
				}
			}
			//Execute instructions at 60fps
			if (accumulator >= tpf)
			{
				try {
					chip->emulate();
				}
				catch (const std::string& msg) {
					window.close();
					std::cerr << msg << std::endl;
				}
				accumulator = sf::Time::Zero;
			}
			//Handle Delay and Sound timers
			if (timerAccumulator >= timerTpf) {
				if (chip->delay_timer > 0) {
					chip->delay_timer--;
				}
				if (chip->sound_timer > 0) {
					/*if (sound.getStatus() != sf::Sound::Playing) {
						sound.play();
					}*/
					chip->sound_timer--;
				}
				else {
					/*if (sound.getStatus() == sf::Sound::Playing) {
						sound.stop();
					}*/
				}

				timerAccumulator = sf::Time::Zero;
			}

			if (chip->draw) {
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
			accumulator += clock.getElapsedTime();
			timerAccumulator += clock.restart();
		}
	}

public:
	unsigned char display[64 * 32];

private:
	void handle_inputs(sf::Event event)
	{

	}

	void clear_screen()
	{
		for (int i = 0; i < 64 * 32; i++)
		{
			chip->gfx[i] = 0;
		}
	}


private:
	chip8* chip;

	float_t pixelscale = 12.0f;

	sf::RenderWindow window;
	sf::Color backgroundColor;
	sf::RectangleShape pixel;
	sf::Clock clock;
	sf::Time tpf = sf::seconds(1.f / 60);
	sf::Time accumulator = sf::Time::Zero;
	sf::Time timerTpf = sf::seconds(1.f / 60.f);
	sf::Time timerAccumulator = sf::Time::Zero;
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