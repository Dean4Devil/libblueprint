#include "blueprint.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "parson.h"
#include "bstrlib.h"

// Parses the blueprint object inside the file-level object
// Returns 0 if successful, 1 if not
int parse_blueprint(const_bstring json, size_t json_length, struct blueprint *bp)
{
    JSON_Value *bp_json;
    JSON_Object *root, *blueprint, *item_dictionary, *version;
    bstring Name, bp_name, name, game_version;
    uint32_t resource_cost[5];

    bp_json = json_parse_string(bstr2cstr(json, '0'));
    if (bp_json == NULL)
        return 1;

    // Simple validation
    if (json_value_get_type(bp_json) != JSONObject)
        return 1;

    root = json_value_get_object(bp_json);

    blueprint = json_object_get_object(root, "Blueprint");
    if (blueprint == NULL)
        return 1;

    Name = bfromcstr(json_object_get_string(root, "Name"));
    bp_name = bfromcstr(json_object_get_string(blueprint, "blueprintName"));
    name = bfromcstr(json_object_get_string(blueprint, "Name"));
    game_version = bfromcstr(json_object_get_string(blueprint, "GameVersion"));

    bp->name = bstrcpy(name);
    bp->blueprint_name = bstrcpy(bp_name);
    bp->Name = bstrcpy(Name);
    bp->game_version = bstrcpy(game_version);

    bdestroy(name);
    bdestroy(bp_name);
    bdestroy(Name);
    bdestroy(game_version);

    bp->revision = (uint32_t) json_object_get_number(root, "Version");

    JSON_Array *jresource_cost = json_object_get_array(blueprint, "ResourceCost");
    for (int i = 0; i < 5; i++)
        bp->resource_cost[i] = (uint32_t) json_array_get_number(jresource_cost, i);

    JSON_Array *min_coords = json_object_get_array(blueprint, "MinCords");
    for (int i = 0; i < 3; i++)
        bp->min_coords[i] = (int32_t) json_array_get_number(min_coords, i);

    JSON_Array *max_coords = json_object_get_array(blueprint, "MaxCords");
    for (int i = 0; i < 3; i++)
        bp->max_coords[i] = (int32_t) json_array_get_number(max_coords, i);


    bp->last_alive_block = (uint32_t) json_object_get_number(blueprint, "LastAliveBlock");

    JSON_Array *palette = json_object_get_array(blueprint, "COL");
    for(int i = 0; i < 32; i++)
    {
        uint32_t rbga = 0;

        bstring color = bfromcstr(json_array_get_string(palette, i));

        // Create array of substrings
        struct bstrList *values = bsplit(color, ',');
        for (int n = 0; n < 4; n++)
        {
            char *ptr;
            const char *str = bstr2cstr(values->entry[n], '0');
            double dbl = strtod(str, &ptr);
            bp->color_palette[i].array[n] = round(dbl * 255.0);
        }
    }

    bp->total_block_count = (uint32_t) json_object_get_number(blueprint, "TotalBlockCount");
    bp->blocks = calloc(bp->total_block_count, sizeof(struct block));

    JSON_Array *material = json_object_get_array(blueprint, "BlockIds");
    JSON_Array *position = json_object_get_array(blueprint, "BLP");
    JSON_Array *rotation = json_object_get_array(blueprint, "BLR");
    JSON_Array *color = json_object_get_array(blueprint, "BCI");

    for (int i = 0; i < bp->total_block_count; i++)
    {
        struct block *act = &bp->blocks[i];

        act->material = (uint32_t) json_array_get_number(material, i);
        act->rotation = (uint32_t) json_array_get_number(rotation, i);
        act->color = (uint32_t) json_array_get_number(color, i);

        bstring pos_string = bfromcstr(json_array_get_string(position, i));

        struct bstrList *pos_list = bsplit(pos_string, ',');
        for (int n = 0; n < 3; n++)
        {
            char *ptr;
            const char *str = bstr2cstr(pos_list->entry[n], '0');
            double dbl = strtod(str, &ptr);
            uint32_t val = round(dbl);
            act->position.array[n] = val;
        }
    }

    // I don't want to handle this case quite yet.
    bp->num_sc = 0;
    bp->SCs = NULL;

    json_value_free(bp_json);
    return 0;
}

void free_blueprint(struct blueprint *bp)
{
    if (bp == NULL)
        return;

    bdestroy(bp->name);
    bdestroy(bp->blueprint_name);
    bdestroy(bp->Name);
    bdestroy(bp->game_version);
    for (int i = 0; i < bp->num_sc; i++)
        free_blueprint(&bp->SCs[i]);
    free(bp);
}
