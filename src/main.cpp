#include "chip8.h"
#include "driver.h"

int main()
{
    chip8 chip_emu;
    Driver screen;

    chip_emu.initialize();
    chip_emu.load_game("test_opcode.ch8");
   
    screen.run(&chip_emu);


    return 0;
}