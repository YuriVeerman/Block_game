#include "../include/window.h"

int WINDOW_HEIGHT = 1000;
int WINDOW_WIDTH = 1000;
float PLAYER_X = 0.0f, PLAYER_Y = 0.0f, PLAYER_Z = 2.0f;
float PLAYER_YAW = 0.0f, PLAYER_PITCH = 0.0f;
float MOUSE_SENSITIVITY = 0.1f;
float MOVEMENT_SPEED = 0.1f;

GLFWwindow *window_init(void) {
    /* Initialize the library */
    if (!glfwInit())
        return NULL;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window;
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // Disable V-Sync
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    return window;
}

int window_draw_block_textured(struct block_lib *block_lib, const char *name, float x, float y, float z) {
    // Find the block by name
    int block_index_pos = blocks_get_index_by_name(block_lib, name);
    int block_lib_capacity = block_lib_get_capacity(block_lib);

    if (block_index_pos == block_lib_capacity) {
        printf("Block with name '%s' not found!\n", name);
        return 1;
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


int window_player_move(int direction) {
    float off_x;
    float off_z;

    float movement_angle = PLAYER_YAW + direction;
    float rad_angle = (movement_angle - 90) * M_PI / 180.0;

    off_x = cos(rad_angle) * MOVEMENT_SPEED;
    off_z = sin(rad_angle) * MOVEMENT_SPEED;

    PLAYER_X += off_x;
    PLAYER_Z += off_z;

    return 0;
}

int handle_player_control_input(GLFWwindow *window) {
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
        PLAYER_Y += MOVEMENT_SPEED;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        PLAYER_Y -= MOVEMENT_SPEED;
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

    PLAYER_YAW += xoffset * MOUSE_SENSITIVITY;
    PLAYER_PITCH += yoffset * MOUSE_SENSITIVITY;

    return 0;
}

int window_setup_game(GLFWwindow *window) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float)width / (float)height;

    float fov = 60.0f;
    float fH = tan(fov / 360.0f * 3.14159f) * 0.1f;
    float fW = fH * aspect;
    float zNear = 0.1f;
    float zFar = 100.0f;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);

    // Hide and lock cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0);

    return 0;
}

enum game_state window_game_loop(struct block_lib* block_lib, GLFWwindow *window) {
    /* Render here */
    glLoadIdentity();
    glRotatef(PLAYER_PITCH, 1, 0, 0);                   // update camera pitch
    glRotatef(PLAYER_YAW, 0, 1, 0);                     // update camera yaw
    glTranslatef(-PLAYER_X, PLAYER_Y, -PLAYER_Z);       // Update world pos relative to camera.

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // dark teal background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    window_draw_block_textured(block_lib, "cobblestone", 1.0f, 0.0f, 0.0f);
    window_draw_block_textured(block_lib, "water", 2.0f, 0.0f, 0.0f);
    window_draw_block_textured(block_lib, "hi", 3.0f, 0.0f, 0.0f);
    window_draw_block_textured(block_lib, "cat", 4.0f, 0.0f, 0.0f);
    window_draw_block_textured(block_lib, "steven", 5.0f, 0.0f, 0.0f);

    // for game nuking fun
    for (int x = 0; x < 20; x++) {
        for (int y = 0; y < 20; y++) {
            for (int z = 0; z < 20; z++) {
                window_draw_block_textured(block_lib, "steven", x + 10, y + 10, x + 10);
            }
        }
    }

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
    handle_player_control_input(window);
    handle_mouse_input(window);


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        return PAUSED_START;
    }

    return GAME;
}

void window_setup_static(GLFWwindow *window) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPos(window, WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0);
}

enum game_state window_main_menu(GLFWwindow *window) {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // menu background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Simple menu rendering (text placeholder)
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.2f, 0.1f);
    const char *msg1 = "MAIN MENU";
    for (const char *c = msg1; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glRasterPos2f(-0.2f, -0.1f);
    const char *msg2 = "Press O to Open World";
    for (const char *c = msg2; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glRasterPos2f(-0.2f, -0.2f);
    const char *msg3 = "Press Q to Quit Game";
    for (const char *c = msg3; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    // Handle input
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        return GAME_START;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        return EXIT;
    }
    return MENU; // Stay in menu
}

enum game_state window_pause_menu(GLFWwindow *window) {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // menu background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Simple menu rendering (text placeholder)
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-0.2f, 0.1f);
    const char *msg1 = "PAUSE MENU";
    for (const char *c = msg1; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glRasterPos2f(-0.2f, -0.1f);
    const char *msg2 = "Press O to Resume Playing";
    for (const char *c = msg2; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glRasterPos2f(-0.2f, -0.2f);
    const char *msg3 = "Press M to go to Main Menu";
    for (const char *c = msg3; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glRasterPos2f(-0.2f, -0.3f);
    const char *msg4 = "Press Q to Quit Game";
    for (const char *c = msg4; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    // Handle input
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        return GAME_START; // change to resume later!
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        return MENU_START; // change to resume later!
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        return EXIT;
    }

    return PAUSED;
}
