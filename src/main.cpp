#include "chip8.h"
#include "driver.h"

int main(int argc, char* argv[])
{
    const char* file_path = "tests/ibm.ch8";

    if (argc <= 1)
    {
        std::cout << "Path to ROM to be loaded must be given as argument\nType -help to see usage\n"
            << "Defaulting to IBM logo (tests/ibm.ch8)" << std::endl;
    }
    else if (argc == 2)
    {
        puts(argv[1]);
        file_path = argv[1];
    }
    else if (argc > 2)
    {
        std::cout << "Path to ROM to be loaded must be given as argument\nType -help to see usage\n"
            << "Defaulting to IBM logo (tests/ibm.ch8)" << std::endl;
    }

    chip8 chip_emu;
    Driver screen;

    chip_emu.initialize();
    chip_emu.load_game(file_path);
   
    screen.run(&chip_emu);


    return 0;
}