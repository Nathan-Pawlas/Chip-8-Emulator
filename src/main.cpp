#include "chip8.h"
#include "driver.h"

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cerr << "Path to ROM to be loaded must be given as argument\nType -help to see usage\n";
        exit(1);
    }

    if (argv[0] == "-help")
    {
        std::cout << "Normal usage: ./Chip8Emu <path_to_rom>\n"
            << "example path: ./Chip8Emu tests\ibm.ch8\n" << std::endl;
    }

    chip8 chip_emu;
    Driver screen;

    chip_emu.initialize();
    chip_emu.load_game(argv[0]);
   
    screen.run(&chip_emu);


    return 0;
}