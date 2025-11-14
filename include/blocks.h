#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include "textures.h"
#include "cJSON.h"

#define MAX_BLOCK_NAME_SIZE 64
#define TEXTURES_PER_BLOCK 6


struct state {
    GLint gl_id[6];
    char name[MAX_BLOCK_NAME_SIZE];
};

struct block {
    struct state *states;
    int states_amount;
    char name[MAX_BLOCK_NAME_SIZE];
    int id;
};

struct block_lib {
    struct block *blocks;
    int capacity;
    int count;
};

// init function
struct block_lib *blocks_init(void);
// block_lib_deconstruct function to be made here!!

// get options
int blocks_get_index_by_name(struct block_lib *block_lib, const char *name);
int block_lib_get_capacity(struct block_lib *block_lib);
GLint block_get_gl_id(struct block_lib *block_lib, int index);
