#pragma once
#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "blocks.h"
#include <GLUT/glut.h>

enum game_state { MENU, MENU_START, GAME, GAME_START, GAME_RESUME, PAUSED, PAUSED_START, EXIT };

GLFWwindow *window_init(void);
enum game_state window_game_loop(struct block_lib *block_lib, GLFWwindow *window);
enum game_state window_main_menu(GLFWwindow *window);
enum game_state window_pause_menu(GLFWwindow *window);

int window_setup_game(GLFWwindow *window);
void window_setup_static(GLFWwindow *window);
