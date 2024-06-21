#include <GLFW/glfw3.h>
#include <stdlib.h>
#include "chip8.h"


GLFWwindow* initWindow()
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        exit(-1);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 320, "Chip8-Emu", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    return window;
}

int main(void)
{
    GLFWwindow* window = initWindow();

    chip8 chip8_emu;

    chip8_emu.initialize();

    //chip8_emu.load_game("IBM Logo.ch8");
    
    chip8_emu.emulate();

    return 0;
}

/* Loop until the user closes the window */
    //while (!glfwWindowShouldClose(window))
    //{
    //    /* Render here */
    //    glClear(GL_COLOR_BUFFER_BIT);

    //    /* Swap front and back buffers */
    //    glfwSwapBuffers(window);

    //    /* Poll for and process events */
    //    glfwPollEvents();
    //}

    //glfwTerminate();
