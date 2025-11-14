#include "../include/blocks.h"


void block_init_state(struct texture_lib *texture_lib, cJSON *state_json, struct state *state) {
    // get state name
    char *state_name = cJSON_GetObjectItem(state_json, "name")->valuestring;

    // get textures
    cJSON *textures_json = cJSON_GetObjectItem(state_json, "textures");

    // check for other texture types at some point here!!

    // check for all;
    cJSON *texture_name_pointer = cJSON_GetObjectItem(textures_json, "all");
    char *texture_name;
    if (texture_name_pointer == NULL) {
        printf("all category not found within textures\n");
    } else {
        texture_name = cJSON_GetObjectItem(textures_json, "all")->valuestring;
        printf("all category found! texture name in it is: %s\n", texture_name);
    }

    // set values
    memcpy(state->name, state_name, MAX_BLOCK_NAME_SIZE - 2);
    state->name[MAX_BLOCK_NAME_SIZE - 1] = '\0';

    // set for every texture!
    for (int i = 0; i < TEXTURES_PER_BLOCK; i++) {
        // get texture from index
        int texture_index = texture_get_index_by_name(texture_lib, texture_name);

        // input all values
        // int channels = texture_get_channels(texture_lib, texture_index);
        // int size = texture_get_size(texture_lib, texture_index);

        // state->textures[i].channels = channels;
        // state->textures[i].size = size;
        // memcpy(state->textures[i].name, texture_get_name(texture_lib, texture_index), MAX_BLOCK_NAME_SIZE);
        // state->textures[i].pixels = malloc(size * size * channels);
        // if (state->textures[i].pixels == NULL) {
        //     printf("Failed to allocate memory for texture pixels\n");
        //     // Handle error (e.g., return or continue)
        // }
        // unsigned char *pixels = texture_get_pixels(texture_lib, texture_index);
        // memcpy(state->textures[i].pixels, pixels, size * size * channels);
        // printf("state->textures[i].channels = %d, .size = %d, .name = %s\n", state->textures[i].channels, size, state->textures[i].name);

        state->gl_id[i] = texture_get_gl_id(texture_lib, texture_index);
    }

    // print values for test
    printf("state name is %s\n", state->name);
}

void block_init(struct texture_lib *texture_lib, cJSON *block_json, struct block *block, cJSON *states_json, int states_amount, const char *name, int id) {
    block->states_amount = states_amount;
    memcpy(block->name, name, MAX_BLOCK_NAME_SIZE - 2);
    block->name[MAX_BLOCK_NAME_SIZE - 1] = '\0';
    block->id = id;

    block->states = malloc(sizeof(struct state) * block->states_amount);
    if (block->states == NULL) {
        return;
    }

    printf("block initiated. id = %d, name = %s, states_amount = %d\n", block->id, block->name, block->states_amount);

    for (int i = 0; i < states_amount; i++) {
        cJSON *state_json = cJSON_GetArrayItem(states_json, i);

        block_init_state(texture_lib, state_json, &block->states[i]);
    }
}

void blocks_init_every_block(cJSON *root, cJSON *blocks_array, int blocks_count, struct block_lib *block_lib, struct texture_lib *texture_lib) {
    for (int i = 0; i < blocks_count; i++) {
        // get block and block name
        cJSON *block_json = cJSON_GetArrayItem(blocks_array, i);
        cJSON *name_json = cJSON_GetObjectItem(block_json, "name");
        const char *name = name_json ? name_json->valuestring : "(null)";
        printf("blocks_array item %d is %s\n", i, name);

        // get block states amount
        cJSON *states_json = cJSON_GetObjectItem(block_json, "states");
        int states_amount = cJSON_GetArraySize(states_json);

        // get block id
        int id = cJSON_GetObjectItem(block_json, "id")->valueint;
        block_init(texture_lib, block_json, &block_lib->blocks[i], states_json, states_amount, name, id);
    }
}

struct block_lib *block_lib_init(int size) {
    struct block_lib *block_lib = malloc(sizeof(struct block_lib));
    if (block_lib == NULL) {
        return NULL;
    }

    block_lib->capacity = size;
    block_lib->count = 0;

    block_lib->blocks = malloc(sizeof(struct block) * block_lib->capacity);
    if (block_lib->blocks == NULL) {
        free(block_lib);
        return NULL;
    }

    return block_lib;
}

struct block_lib *blocks_init(void) {
    // open blocks file and get file size(blocks_file_length).
    FILE *blocks_file = fopen("assets/blocks.json", "r");
    fseek(blocks_file, 0, SEEK_END);
    int blocks_file_length = ftell(blocks_file);
    fseek(blocks_file, 0, SEEK_SET);

    // create an array that holds the data.
    char *data = malloc(blocks_file_length + 1);
    if (data == NULL) {
        return NULL; // change later with return type!!!
    }
    fread(data, 1, blocks_file_length, blocks_file);
    data[blocks_file_length] = '\0';
    fclose(blocks_file);

    // parse the json data
    cJSON *root = cJSON_Parse(data);
    free(data);

    // get size for block library
    cJSON *blocks_array = cJSON_GetObjectItem(root, "blocks");
    int block_count = cJSON_GetArraySize(blocks_array);

    // make block library and texture library
    struct block_lib *block_lib = block_lib_init(block_count);
    struct texture_lib *texture_lib = textures_init();

    printf("testing\n");

    // enter all blocks;
    blocks_init_every_block(root, blocks_array, block_count, block_lib, texture_lib);

    // free texture library
    textures_release(texture_lib);
    return block_lib;
}



int blocks_get_index_by_name(struct block_lib *block_lib, const char *name) {
    int itterate = block_lib->capacity;
    int i;
    for (i = 0; i < itterate; i++) {
        if (strcmp(block_lib->blocks[i].name, name) == 0) {
            break;
        }
    }

    return i;
}

int block_lib_get_capacity(struct block_lib *block_lib) {
    if (block_lib == NULL) {
        return 0;
    }

    return block_lib->capacity;
}

GLint block_get_gl_id(struct block_lib *block_lib, int index) {
    return block_lib->blocks[index].states[0].gl_id[0];
}
