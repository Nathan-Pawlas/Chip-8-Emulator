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

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320

class Screen
{
public:
	Screen();
	~Screen();

	void initialize(chip8* cpu)
	{
		chip = cpu;

		sf::RenderWindow window(sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Chip8", sf::Style::Close));
		backgroundColor = sf::Color().Black;
		pixel.setSize(sf::Vector2f(pixelscale, pixelscale));
		pixel.setFillColor(sf::Color().White);

		clear_screen();

		//Load fonts
		for (int i = 0; i < 80; i++)
		{
			chip->memory[i] = fonts[i];
		}
	}

	void run()
	{
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

			if (chip->reg[0xF]) {
				window.clear(backgroundColor);

				for (std::size_t x= 0; x< 64; x++) {
					for (std::size_t y= 0; y< 128; y++) {
						if (gfx[y+ x* 64] == 1) {
							/*std::uint8_t r = rand() % 255;
							std::uint8_t g = rand() % 255;
							std::uint8_t b = rand() % 255;
							pixel.setFillColor(sf::Color(r, g, b));*/
							pixel.setPosition(pixelscale * y, pixelscale * x);
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
	unsigned char gfx[64 * 32];

	//Fonts are 4x5 pixels
	unsigned char fonts[80] =
	{
		0x60, 0xB0, 0xB0, 0xB0, 0x60, //0
		0xF0, 0x30, 0x30, 0x30, 0x78, //1
		0xF0, 0x90, 0x20, 0x40, 0xF0, //2
		0xF0, 0x10, 0x70, 0x10, 0xF0, //3
		0x90, 0x90, 0xF0, 0x10, 0x10, //4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
		0xF0, 0x90, 0x38, 0x10, 0x10, //7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
		0xF0, 0x90, 0xF0, 0x10, 0x10, //9
		0x60, 0x90, 0xF0, 0x90, 0x90, //A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
		0xF0, 0x80, 0x80, 0x80, 0xF0, //C
		0xE0, 0x90, 0x90, 0x90, 0xE0, //E
		0xF0, 0x80, 0xF0, 0x80, 0x80, //F
	};

private:
	void handle_inputs(sf::Event event)
	{

	}

	void clear_screen()
	{
		for (int i = 0; i < 64 * 32; i++)
		{
			gfx[i] = 0;
		}
	}


private:
	chip8* chip;

	float_t pixelscale = 8.0f;

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