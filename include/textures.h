#include "stb_image.h"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define FILEPATH_MAX_SIZE 256


struct texture {
    unsigned char* pixels;
    char name[FILEPATH_MAX_SIZE];
    int channels;
    int size;
    GLuint gl_id;
};

struct texture_lib {
    struct texture *textures;
    int capacity;
    int count;
};

// makes and returns a texture library with above structure.
struct texture_lib *textures_init(void);
// free's all elements of a given texture library
void textures_release(struct texture_lib *texture_lib);

// get options. function is self explanatory. no side effects
int texture_get_index_by_name(struct texture_lib *texture_lib, const char *name);
int texture_lib_get_capacity(struct texture_lib *texture_lib);
unsigned char *texture_get_pixels(struct texture_lib *texture_lib, int index);
int texture_get_channels(struct texture_lib *texture_lib, int index);
int texture_get_size(struct texture_lib *texture_lib, int index);
char *texture_get_name(struct texture_lib *texture_lib, int index);
GLuint texture_get_gl_id(struct texture_lib *texture_lib, int index);
