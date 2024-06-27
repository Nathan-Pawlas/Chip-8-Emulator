#include "chip8.h"
#include "driver.h"

int main(int argc, char* argv[])
{
    const char* file_path;

    if (argc <= 1)
    {
        std::cout << "Path to ROM to be loaded must be given as argument\nType -help to see usage\n"
            << "Defaulting to IBM logo (tests/ibm.ch8)" << std::endl;
        file_path = "tests/ibm.ch8";
    }
    else if (argv[0] == "-help")
    {
        std::cout << "Normal usage: ./Chip8Emu <path_to_rom>\n"
            << "example path: ./Chip8Emu tests/ibm.ch8\n" << std::endl;
        file_path = "tests/ibm.ch8";
    }
    else
    {
        file_path = argv[0];
    }

    chip8 chip_emu;
    Driver screen;

    chip_emu.initialize();
    chip_emu.load_game(file_path);
   
    screen.run(&chip_emu);


    return 0;
}