#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "../include/window.h"

int main(void) {
    GLFWwindow *window = window_init();
    struct block_lib *block_lib = blocks_init();
    window_frame_loop(block_lib, window);

    return 0;
}
