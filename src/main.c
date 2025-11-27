#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "../include/window.h"

void gameloop(GLFWwindow *window, struct block_lib *block_lib) {
    enum game_state state = MENU_START;

    // frame checking
    double lastTime = glfwGetTime();
    int frames = 0;

    while (!glfwWindowShouldClose(window)) {
        if (state == MENU || state == MENU_START) {
            if (state == MENU_START) {
                window_setup_static(window);
                state = MENU;
            }
            state = window_main_menu(window);
        } else if (state == GAME || state == GAME_START) {
            if (state == GAME_START || state == GAME_RESUME) {
                window_setup_game(window);
                state = GAME;
            }
            if (state == GAME_START) {
                // action for retrieving world.
            }
            state = window_game_loop(block_lib, window);
        } else if (state == PAUSED || state == PAUSED_START) {
            if (state == PAUSED_START) {
                window_setup_static(window);
                state = PAUSED;
            }
            state = window_pause_menu(window);
        } else if (state == EXIT) {
            printf("game exit\n");
            break;
        }

        // frame tracking
        frames++;
        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= 1.0) {
            printf("FPS: %d\n", frames);
            frames = 0;
            lastTime = currentTime;
        }
    }
}

int main(void) {
    GLFWwindow *window = window_init();
    struct block_lib *block_lib = blocks_init();

    gameloop(window, block_lib);

    glfwTerminate();
    return 0;
}
