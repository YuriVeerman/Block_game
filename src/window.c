#include "../include/window.h"

int SCREEN_HEIGHT = 1000;
int SCREEN_WIDTH = 1000;

float CAM_X = 0.0f, CAM_Y = 0.0f, CAM_Z = 2.0f;

float CAM_YAW = 0.0f, CAM_PITCH = 0.0f;

float MOUSE_SENSITIVITY = 0.1f;

float MOVEMENT_SPEED = 0.1f;


void setup_projection(float aspect) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float fov = 60.0f;
    float fH = tan(fov / 360.0f * 3.14159f) * 0.1f;
    float fW = fH * aspect;
    float zNear = 0.1f;
    float zFar = 100.0f;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

GLFWwindow *window_init(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return NULL;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Hide and lock cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);

    // Setup 3D perspective
    setup_projection(1.0f);

    return window;
}

int window_draw_block_textured(struct block_lib *block_lib, const char *name, float x, float y, float z) {
    // Find the block by name
    int block_index_pos = blocks_get_index_by_name(block_lib, name);
    int block_lib_capacity = block_lib_get_capacity(block_lib);

    if (block_index_pos == block_lib_capacity) {
        printf("Block with name '%s' not found!\n", name);
        return 1;
    } else {
        printf("block with name %s found at index %d\n", name, block_index_pos);
    }

    GLint gl_id = block_get_gl_id(block_lib, block_index_pos);

    // Bind the OpenGL texture (assumes tex->gl_id is set up)
    glBindTexture(GL_TEXTURE_2D, gl_id);

    glColor3f(1.0f, 1.0f, 1.0f); // Set color to white for full texture brightness

    glBegin(GL_QUADS);

    // Front face (z+)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x-0.5f, y-0.5f, z+0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+0.5f, y-0.5f, z+0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+0.5f, y+0.5f, z+0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x-0.5f, y+0.5f, z+0.5f);

    // Back face (z-)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x-0.5f, y-0.5f, z-0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+0.5f, y-0.5f, z-0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+0.5f, y+0.5f, z-0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x-0.5f, y+0.5f, z-0.5f);

    // Left face (x-)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x-0.5f, y-0.5f, z-0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x-0.5f, y-0.5f, z+0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x-0.5f, y+0.5f, z+0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x-0.5f, y+0.5f, z-0.5f);

    // Right face (x+)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x+0.5f, y-0.5f, z-0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+0.5f, y-0.5f, z+0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+0.5f, y+0.5f, z+0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x+0.5f, y+0.5f, z-0.5f);

    // Top face (y+)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x-0.5f, y+0.5f, z-0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+0.5f, y+0.5f, z-0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+0.5f, y+0.5f, z+0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x-0.5f, y+0.5f, z+0.5f);

    // Bottom face (y-)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x-0.5f, y-0.5f, z-0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x+0.5f, y-0.5f, z-0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x+0.5f, y-0.5f, z+0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x-0.5f, y-0.5f, z+0.5f);

    glEnd();

    return 0;
}

int window_draw_block(float x, float y, float z) {
    glBegin(GL_QUADS);

    // Front face (z+)
    glVertex3f(x-0.5f, y-0.5f, z+0.5f);
    glVertex3f(x+0.5f, y-0.5f, z+0.5f);
    glVertex3f(x+0.5f, y+0.5f, z+0.5f);
    glVertex3f(x-0.5f, y+0.5f, z+0.5f);

    // Back face (z-)
    glVertex3f(x-0.5f, y-0.5f, z-0.5f);
    glVertex3f(x+0.5f, y-0.5f, z-0.5f);
    glVertex3f(x+0.5f, y+0.5f, z-0.5f);
    glVertex3f(x-0.5f, y+0.5f, z-0.5f);

    // Left face (x-)
    glVertex3f(x-0.5f, y-0.5f, z-0.5f);
    glVertex3f(x-0.5f, y-0.5f, z+0.5f);
    glVertex3f(x-0.5f, y+0.5f, z+0.5f);
    glVertex3f(x-0.5f, y+0.5f, z-0.5f);

    // Right face (x+)
    glVertex3f(x+0.5f, y-0.5f, z-0.5f);
    glVertex3f(x+0.5f, y-0.5f, z+0.5f);
    glVertex3f(x+0.5f, y+0.5f, z+0.5f);
    glVertex3f(x+0.5f, y+0.5f, z-0.5f);

    // Top face (y+)
    glVertex3f(x-0.5f, y+0.5f, z-0.5f);
    glVertex3f(x+0.5f, y+0.5f, z-0.5f);
    glVertex3f(x+0.5f, y+0.5f, z+0.5f);
    glVertex3f(x-0.5f, y+0.5f, z+0.5f);

    // Bottom face (y-)
    glVertex3f(x-0.5f, y-0.5f, z-0.5f);
    glVertex3f(x+0.5f, y-0.5f, z-0.5f);
    glVertex3f(x+0.5f, y-0.5f, z+0.5f);
    glVertex3f(x-0.5f, y-0.5f, z+0.5f);
    glEnd();

    return 0;
}


int window_player_move(int direction) {
    float off_x;
    float off_z;

    float movement_angle = CAM_YAW + direction;
    float rad_angle = (movement_angle - 90) * M_PI / 180.0;

    off_x = cos(rad_angle) * MOVEMENT_SPEED;
    off_z = sin(rad_angle) * MOVEMENT_SPEED;

    CAM_X += off_x;
    CAM_Z += off_z;

    return 0;
}

int handle_key_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        window_player_move(-90);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        window_player_move(90);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        window_player_move(0);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        window_player_move(180);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        CAM_Y += 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        CAM_Y -= 0.1f;
    }
}

int handle_mouse_input(GLFWwindow *window) {
    static double last_x = 0.0, last_y = 0.0;
    static int first_call = 1;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (first_call) {
        last_x = xpos;
        last_y = ypos;
        first_call = 0;
    }

    double xoffset = xpos - last_x;
    double yoffset = ypos - last_y;
    last_x = xpos;
    last_y = ypos;

    CAM_YAW += xoffset * MOUSE_SENSITIVITY;
    CAM_PITCH += yoffset * MOUSE_SENSITIVITY;

    return 0;
}

int window_frame_loop(struct block_lib* block_lib, GLFWwindow *window) {
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glLoadIdentity();
        glRotatef(CAM_PITCH, 1, 0, 0);          // update camera pitch
        glRotatef(CAM_YAW, 0, 1, 0);            // update camera yaw
        glTranslatef(-CAM_X, CAM_Y, -CAM_Z);   // Update world pos relative to camera.

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // dark teal background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window_draw_block(0.0, 0.0, 0.0);
        window_draw_block_textured(block_lib, "cobblestone", 1.0f, 0.0f, 0.0f);
        window_draw_block_textured(block_lib, "water", 2.0f, 0.0f, 0.0f);
        window_draw_block_textured(block_lib, "hi", 3.0f, 0.0f, 0.0f);
        window_draw_block_textured(block_lib, "cat", 4.0f, 0.0f, 0.0f);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        handle_key_input(window);
        handle_mouse_input(window);
    }

    glfwTerminate();
    return 0;
}
