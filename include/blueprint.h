#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "bstrlib.h"

// Struct representing a single block
typedef struct block
{
    // Material of the block according to the ItemDictionary in every BP
    // Saved as Blueprint.BlockIds
    uint32_t material;

    // Relative position of every block to the first placed block
    // Game uses floats (why?) but accepts whole values as well
    // Saved as Blueprint.BLP
    union
    {
        struct
        {
            int32_t x;
            int32_t y;
            int32_t z;
        };

        int32_t array[3];

    } position;

    // Int giving the rotation.
    // Has a value of 0-24, I assume this is 6 * 4, as in
    // (front face (one of six a block has) * rotation)
    // Saved as Blueprint.BLR
    uint8_t rotation;

    // Color of the block.
    // Index for the color_palette value saved in the blueprint
    // Saved as Blueprint.BCI
    uint8_t color;

    // Extranous data for the block
    // (i.e. Lua code for the Lua box)
    // Saved as Blueprint.BlockStringData, which gets
    // an unique ID by Blueprint.BlockStringDataIds,
    // which is used as the fourth value in BP1
    // string_data may be NULL
    bstring string_data;
} BLOCK;

// Struct representing a blueprint
typedef struct blueprint
{
    // Name of the blueprint
    // Saved as Blueprint.Name
    bstring name;
    // Saved as Blueprint.blueprintName
    bstring blueprint_name;
    // Saved as Name
    bstring Name;
    // Why three names you ask?
    // I have no clue either.

    // 5 uints showing the resource cost of
    // Natural, Metal, Oil, Scrap and Crystal respectively
    // Currently not used in-game (no cheating for you! :P)
    // Saved as Blueprint.ResourceCost
    uint32_t resource_cost[5];

    // Color palette this blueprint uses. Saved as RBGA value.
    // 0 - 27: user defined
    // 28: Fleet Detail
    // 29: Fleet Trim
    // 30: Fleet Secondary
    // 31: Fleet Main
    // Saved in Blueprint.COL
    union
    {
        struct
        {
            uint8_t alpha;
            uint8_t blue;
            uint8_t green;
            uint8_t red;
        } color;

        uint8_t array[4];

        uint32_t hex;

    } color_palette[32];

    // Number of blocks in the blueprint and array of blocks
    uint32_t total_block_count;
    struct block* blocks;

    // Saved as Blueprint.MaxCords and Blueprint.MinCords
    int32_t max_coords[3];
    int32_t min_coords[3];

    // The last build block
    uint32_t last_alive_block;

    // String giving the game version this blueprint was last saved under
    // (Compatibiliyt mode maybe?)
    bstring game_version;

    // Sub-Vehicles (only set in a mothership)
    uint32_t num_sc;
    struct blueprint *SCs;

    // Revision of the blueprint
    // Seen in game as the v1 after a blueprint's name
    // Saved as Blueprint.blueprintVersion
    uint32_t revision;
    // Saved as Version
    uint32_t version;
} BLUEPRINT;


int parse_blueprint(bstring, struct blueprint *);
bstring serialize_blueprint(struct blueprint *bp);

bstring get_name(uint32_t key);

void free_blueprint(struct blueprint *);


#ifdef __cplusplus
} // extern "C"
#endif
