#define STB_IMAGE_IMPLEMENTATION
#include "../include/textures.h"


int set_texture(struct texture *texture, int width, int height, int channels, unsigned char *pixels, const char *name) {
    printf("set_texture activated for %s\n", name);
    // validate input is a square
    if (width != height) {
        printf("width(%d) and height(%d) do not line up for texture %s", width, height, name);
        return 0;
    }

    // make room for the pixels
    texture->pixels = malloc(sizeof(unsigned char) * width * height * channels);
    if (texture->pixels == NULL) {
        return 1;
        printf("texture->pixels couldnt be initiated for %s", name);
    }

    // copy over the pixels.
    memcpy(texture->pixels, pixels, width * height * channels);

    glGenTextures(1, &texture->gl_id);
    glBindTexture(GL_TEXTURE_2D, texture->gl_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // set other values
    texture->channels = channels;
    texture->size = width;
    strncpy(texture->name, name, FILEPATH_MAX_SIZE - 1);
    texture->name[FILEPATH_MAX_SIZE - 1] = '\0'; // Ensure null-termination

    return 0;
}

int set_all_textures(struct texture_lib *texture_lib, DIR *texture_dir) {
    struct dirent *texture_entry;

    // loop through every entry in the assets/texture directory
    while((texture_entry = readdir(texture_dir)) != NULL) {
        // check if the current entry is a .png file.
        if (strstr(texture_entry->d_name, ".png")) {
            int width, height, channels;
            int wanted_channels = 4; //RGBA
            char full_filepath[FILEPATH_MAX_SIZE];
            snprintf(full_filepath, sizeof(full_filepath), "assets/textures/%s", texture_entry->d_name);

            unsigned char *pixels = stbi_load(full_filepath, &width, &height, &channels, wanted_channels);
            if (pixels == NULL) {
                printf("couldn't load file %s\n", texture_entry->d_name);
            } else {
                set_texture(&texture_lib->textures[texture_lib->count], width, height, channels, pixels, texture_entry->d_name);
                texture_lib->count++;
            }

            stbi_image_free(pixels);
        }
    }

    return 0;
}

struct texture_lib *texture_lib_init(int size) {
    struct texture_lib *texture_lib = malloc(sizeof(struct texture_lib));
    if (texture_lib == NULL) {
        return NULL;
    }

    texture_lib->capacity = size;
    texture_lib->count = 0;

    texture_lib->textures = malloc(sizeof(struct texture) * size);
    if (texture_lib->textures == NULL) {
        free(texture_lib);
        return NULL;
    }

    printf("texture library initiated succesfully.\n");
    return texture_lib;
}

int dir_get_size(DIR *dir) {
    int count = 0;
    struct dirent *entry;

    while((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".png")) {
            printf("file found with name: %s\n", entry->d_name);
            count++;
        }
    }

    rewinddir(dir);
    return count;
}

struct texture_lib *textures_init(void) {
    // open directory.
    DIR *texture_dir = opendir("assets/textures");
    if (texture_dir == NULL) {
        printf("opening directory failed\n");
        return NULL;
    }

    // get directory size
    int size = dir_get_size(texture_dir);
    printf("size = %d\n", size);

    // init texture library with size;
    struct texture_lib *texture_lib = texture_lib_init(size);

    // set all textures!
    set_all_textures(texture_lib, texture_dir);

    //check values;
    printf("the texture library now has capacity %d, count %d\n", texture_lib->capacity, texture_lib->count);

    closedir(texture_dir);
    return texture_lib;
}

void textures_release(struct texture_lib *texture_lib) {
    int capacity = texture_lib->capacity;

    // free all pixel arrays
    for (int i = 0; i < capacity; i++) {
        free(texture_lib->textures[i].pixels);
    }

    free(texture_lib->textures);
    free(texture_lib);
}

int texture_get_index_by_name(struct texture_lib *texture_lib, const char *name) {
    int itterate = texture_lib->capacity;
    int i;
    for (i = 0; i < itterate; i++) {
        if (strcmp(texture_lib->textures[i].name, name) == 0) {
            printf("texture from blocks.json found in texture library! at name %s\n", name);
            break;
        }
    }

    return i;
}

int texture_lib_get_capacity(struct texture_lib *texture_lib) {
    if (texture_lib == NULL) {
        return 0;
    }

    return texture_lib->capacity;
}

unsigned char *texture_get_pixels(struct texture_lib *texture_lib, int index) {
    if (texture_lib == NULL) {
        return NULL;
    }

    return texture_lib->textures[index].pixels;
}

int texture_get_channels(struct texture_lib *texture_lib, int index) {
    if (texture_lib == NULL) {
        return 0;
    }

    return texture_lib->textures[index].channels;
}

int texture_get_size(struct texture_lib *texture_lib, int index) {
    if (texture_lib == NULL) {
        return 0;
    }

    return texture_lib->textures[index].size;
}

char *texture_get_name(struct texture_lib *texture_lib, int index) {
    if (texture_lib == NULL) {
        return NULL;
    }

    return texture_lib->textures[index].name;
}

GLuint texture_get_gl_id(struct texture_lib *texture_lib, int index) {
    if (texture_lib == NULL) {
        return 0;
    }

    return texture_lib->textures[index].gl_id;
}
