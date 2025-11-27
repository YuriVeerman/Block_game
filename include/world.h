#include <stdio.h>
#include "blocks.h"

#define MAX_BLOCKS_IN_WORLD 1000

struct world_block {
    char name[MAX_BLOCK_NAME_SIZE];
    int x, y, z;
    int state;
};

struct world {
    struct world_block blocks[MAX_BLOCKS_IN_WORLD];
    int count;
};

// create empty world struct
// read in things from world.json file
//
// add blocks to it.