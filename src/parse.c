#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "parson.h"
#include "bstrlib.h"
#include "blueprint.h"

int parse_blueprint_json(JSON_Object *blueprint, struct blueprint *bp)
{
    bstring Name, bp_name, name, game_version;
    uint32_t resource_cost[5];

    bp->revision = json_object_get_number(blueprint, "blueprintVersion");

    bp_name = bfromcstr(json_object_get_string(blueprint, "blueprintName"));
    name = bfromcstr(json_object_get_string(blueprint, "Name"));
    game_version = bfromcstr(json_object_get_string(blueprint, "GameVersion"));

    bstring param1 = bfromcstr(json_object_get_string(blueprint, "Parameter1"));
    bstring param2 = bfromcstr(json_object_get_string(blueprint, "Parameter2"));
    bstring lpos = bfromcstr(json_object_get_string(blueprint, "LocalPosition"));
    bstring lrot = bfromcstr(json_object_get_string(blueprint, "LocalRotation"));

    bp->name = name;
    bp->blueprint_name = bp_name;
    bp->Name = Name;
    bp->game_version = game_version;

    const char* cpos = json_object_get_string(blueprint, "LocalPosition");
    bstring pos = bfromcstr(cpos);
    struct bstrList *pval = bsplit(pos, ',');
    for (int i = 0; i < 3; i++)
    {
        char *ptr;
        char *str = bstr2cstr(pval->entry[i], '0');
        uint32_t n = strtol(str, &ptr, 10);
    }

    const char* crot = json_object_get_string(blueprint, "LocalRotation");
    bstring rot = bfromcstr(crot);
    struct bstrList *rval = bsplit(rot, ',');
    for (int i = 0; i < 4; i++)
    {
        char *ptr;
        char *str = bstr2cstr(rval->entry[i], '0');
        double dbl = strtod(str, &ptr);
        free(str);
        bp->local_rotation[i] = dbl;
    }

    bp->parameter1 = param1;
    bp->parameter2 = param2;

    bp->design_changed = json_object_get_boolean(blueprint, "designChanged");

    bp->id = json_object_get_number(blueprint, "Id");
    bp->force_id = json_object_get_number(blueprint, "ForceId");
    bp->item_number = json_object_get_number(blueprint, "ItemNumber");

    JSON_Array *jresource_cost = json_object_get_array(blueprint, "ResourceCost");
    for (int i = 0; i < 5; i++)
        bp->resource_cost[i] = (uint32_t) json_array_get_number(jresource_cost, i);

    JSON_Array *min_coords = json_object_get_array(blueprint, "MinCords");
    for (int i = 0; i < 3; i++)
        bp->min_coords[i] = (int32_t) json_array_get_number(min_coords, i);

    JSON_Array *max_coords = json_object_get_array(blueprint, "MaxCords");
    for (int i = 0; i < 3; i++)
        bp->max_coords[i] = (int32_t) json_array_get_number(max_coords, i);

    JSON_Array *csi = json_object_get_array(blueprint, "CSI");
    for (int i = 0; i < 40; i++)
        bp->constructable_special_info[i] = json_array_get_number(csi, i);

    bp->last_alive_block = (uint32_t) json_object_get_number(blueprint, "LastAliveBlock");

    JSON_Array *palette = json_object_get_array(blueprint, "COL");
    for(int i = 0; i < 32; i++)
    {
        uint32_t rbga = 0;

        const char* ccolor = json_array_get_string(palette, i);
        bstring color = bfromcstr(ccolor);

        // Create array of substrings
        struct bstrList *values = bsplit(color, ',');
        for (int n = 0; n < 4; n++)
        {
            char *ptr;
            char *str = bstr2cstr(values->entry[n], '0');
            double dbl = strtod(str, &ptr);
            free(str);
            bp->color_palette[i].array[n] = round(dbl * 255.0);
        }
        bstrListDestroy(values);
        bdestroy(color);
    }

    JSON_Array *material = json_object_get_array(blueprint, "BlockIds");
    JSON_Array *position = json_object_get_array(blueprint, "BLP");
    JSON_Array *rotation = json_object_get_array(blueprint, "BLR");
    JSON_Array *color = json_object_get_array(blueprint, "BCI");

    JSON_Array *bp1 = json_object_get_array(blueprint, "BP1");
    JSON_Array *bp2 = json_object_get_array(blueprint, "BP2");
    JSON_Array *block_string_ids = json_object_get_array(blueprint, "BlockStringDataIds");
    JSON_Array *block_data = json_object_get_array(blueprint, "BlockStringData");

    bp->total_block_count = (uint32_t) json_object_get_number(blueprint, "TotalBlockCount");
    bp->main_block_count = (uint32_t) json_object_get_number(blueprint, "BlockCount");
    bp->blocks = calloc(bp->main_block_count, sizeof(struct block));

    for (int i = 0; i < bp->main_block_count; i++)
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
            char *str = bstr2cstr(pos_list->entry[n], '0');
            double dbl = strtod(str, &ptr);
            uint32_t val = round(dbl);
            act->position.array[n] = val;
            free(str);
        }
        bstrListDestroy(pos_list);
        bdestroy(pos_string);

        bstring bp1_string = bfromcstr(json_array_get_string(bp1, i));
        struct bstrList *bp1_values = bsplit(bp1_string, ',');
        for (int n = 0; n < 4; n++)
        {
            char *ptr;
            char *str = bstr2cstr(bp1_values->entry[n], '0');
            double dbl = strtod(str, &ptr);
            act->bp1[n] = dbl;
        }

        bstring bp2_string = bfromcstr(json_array_get_string(bp2, i));
        struct bstrList *bp2_values = bsplit(bp2_string, ',');
        for (int n = 0; n < 4; n++)
        {
            char *ptr;
            char *str = bstr2cstr(bp2_values->entry[n], '0');
            double dbl = strtod(str, &ptr);
            uint32_t val = round(dbl);
            act->bp2[n] = val;
        }

        // Only used for lookup, not saved after that since it has no further semantic value
        const char *cb1 = json_array_get_string(bp1, i);

        if (act->bp1[3] != 0)
        {
            for (int n = 0; n < json_array_get_count(block_string_ids); n++)
            {
                uint32_t test_id = json_array_get_number(block_string_ids, n);
                if (test_id == 0)
                {
                    break;
                }
                if (test_id == act->bp1[3])
                {
                    // BlockStringData at index n is the one we want
                    act->string_data = bfromcstr(json_array_get_string(block_data, n));
                }
            }
        }
    }

    JSON_Array *subconstructables = json_object_get_array(blueprint, "SCs");
    bp->num_sc = json_array_get_count(subconstructables);
    bp->SCs = calloc(bp->num_sc, sizeof(struct blueprint));
    for (int i = 0; i < bp->num_sc; i++)
    {
        JSON_Value *sc_json = json_array_get_value(subconstructables, i);
        JSON_Object *sc_obj = json_value_get_object(sc_json);
        int retval = parse_blueprint_json(sc_obj, &bp->SCs[i]);
        if (retval != 0)
            return retval;
    }

    return 0;
}

int parse_blueprint_root(JSON_Object *root, struct blueprint *bp)
{
    bp->version = json_object_get_number(root, "Version");

    bp->Name = bfromcstr(json_object_get_string(root, "Name"));

    JSON_Object *blueprint = json_object_get_object(root, "Blueprint");
    if (blueprint == NULL)
        return 1;

    int retval;
    if ((retval = parse_blueprint_json(blueprint, bp)) != 0)
        return retval;

    return 0;
}

// Parses the blueprint object inside the file-level object
// Returns 0 if successful, 1 if not
int parse_blueprint(bstring json, struct blueprint *bp)
{
    JSON_Value *bp_json;
    JSON_Object *root;

    char *cjson = bstr2cstr(json, '0');
    bp_json = json_parse_string(cjson);
    free(cjson);
    if (bp_json == NULL)
        return 1;

    root = json_value_get_object(bp_json);
    int retval;
    if ((retval = parse_blueprint_root(root, bp)) != 0)
    {
        json_value_free(bp_json);
        return retval;
    }

    json_value_free(bp_json);
    return 0;
}
