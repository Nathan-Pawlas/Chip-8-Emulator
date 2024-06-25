#include "chip8.h"
#include "screen.h"

int main()
{
    chip8 chip_emu;
    Screen screen;

    chip_emu.initialize();
    chip_emu.load_game("ibm.ch8");
   
    screen.run(&chip_emu);


    return 0;
}