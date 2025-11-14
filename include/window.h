#pragma once
#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "blocks.h"

GLFWwindow *window_init(void);
int window_draw_block(float x, float y, float z);
int window_frame_loop(struct block_lib *block_lib, GLFWwindow *window);
