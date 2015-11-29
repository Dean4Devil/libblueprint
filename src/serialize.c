#include "blueprint.h"
#include "parson.h"
#include "item_dictionary.h"

bstring serialize_blueprint(struct blueprint *bp)
{
    JSON_Value *joutput_val = json_value_init_object();
    JSON_Object *output = json_value_get_object(joutput_val);
    char *serialized_string = NULL;
    bstring output_str;

    // Name
    json_object_set_string(output, "Name", (char *) bp->Name->data);

    // Version
    json_object_set_number(output, "Version", bp->version);

    // ItemDictionary
    {
        JSON_Value *value = json_value_init_object();
        JSON_Object *object = json_object(value);

        serialize_item_dictionary(object);

        json_object_set_value(output, "ItemDictionary", value);
    }

    // FileModelVersion
    {
        JSON_Value *value = json_value_init_object();
        JSON_Object *object = json_object(value);

        json_object_set_number(object, "Major", 1);
        json_object_set_number(object, "Minor", 0);

        json_object_set_value(output, "FileModelVersion", value);
    }

    // Blueprint
    {
        JSON_Value *value = json_value_init_object();
        JSON_Object *object = json_object(value);

        json_object_set_string(object, "LocalRotation", "0,0,0,0");
        json_object_set_string(object, "LocalPosition", "0,0,0,0");
        json_object_set_string(object, "Parameter1", "1,1,1,0");
        json_object_set_string(object, "Parameter2", "0,0,2,1");
        json_object_set_number(object, "ItemNumber", 60);
        json_object_set_boolean(object, "designChanged", false);
        json_object_set_number(object, "Id", 668737);
        json_object_set_number(object, "ForceId", 1502384072);

        json_object_set_number(object, "BlockCount", bp->total_block_count);
        json_object_set_number(object, "TotalBlockCount", bp->total_block_count);
        json_object_set_number(object, "LastAliveBlock", bp->last_alive_block);
        json_object_set_number(object, "blueprintVersion", bp->revision);

        json_object_set_string(object, "blueprintName", (char *) bp->blueprint_name->data);
        json_object_set_string(object, "Name", (char *) bp->name->data);
        json_object_set_string(object, "GameVersion", (char *) bp->game_version->data);

        JSON_Value *cost_val = json_value_init_array();
        JSON_Array *cost = json_array(cost_val);
        for (int i = 0; i < 5; i++)
            json_array_append_number(cost, bp->resource_cost[i]);
        json_object_set_value(object, "ResourceCost", cost_val);

        JSON_Value *scs = json_value_init_array();
        json_object_set_value(object, "SCs", scs);

        JSON_Value *bei = json_value_init_array();
        json_object_set_value(object, "BEI", bei);

        JSON_Value *csi = json_value_init_array();
        json_object_set_value(object, "CSI", csi);

        // Min- / Max Coordinates
        {
            char buffer[50];
            snprintf(buffer, 50, "%d,%d,%d",
                     bp->max_coords[0],
                     bp->max_coords[1],
                     bp->max_coords[2]
            );
            json_object_set_string(object, "MaxCords", buffer);
        }

        {
            char buffer[50];
            snprintf(buffer, 50, "%d,%d,%d",
                     bp->min_coords[0],
                     bp->min_coords[1],
                     bp->min_coords[2]
            );
            json_object_set_string(object, "MinCords", buffer);
        }

        JSON_Value *palette_val = json_value_init_array();
        JSON_Array *palette = json_array(palette_val);
        for (int i = 0; i < 32; i++)
        {
            double r,g,b,a;
            // Get color values in percent
            r = (double) bp->color_palette[i].color.red   / 256;
            g = (double) bp->color_palette[i].color.green / 256;
            b = (double) bp->color_palette[i].color.blue  / 256;
            a = (double) bp->color_palette[i].color.alpha / 256;

            char string[20];
            snprintf(string, 20, "%0.2f,%.2f,%.2f,%.2f", r,b,g,a);
            json_array_append_string(palette, string);
        }
        json_object_set_value(object, "COL", palette_val);

        JSON_Value *material_val = json_value_init_array();
        JSON_Value *position_val = json_value_init_array();
        JSON_Value *rotation_val = json_value_init_array();
        JSON_Value *color_val = json_value_init_array();
        JSON_Value *bp1_val = json_value_init_array();
        JSON_Value *bp2_val = json_value_init_array();
        JSON_Array *material = json_array(material_val);
        JSON_Array *position = json_array(position_val);
        JSON_Array *rotation = json_array(rotation_val);
        JSON_Array *color = json_array(color_val);
        JSON_Array *bp1 = json_array(bp1_val);
        JSON_Array *bp2 = json_array(bp2_val);

        JSON_Value *data_val = json_value_init_array();
        JSON_Value *data_id_val = json_value_init_array();
        JSON_Array *data = json_array(data_val);
        JSON_Array *data_id = json_array(data_id_val);

        uint32_t counter = 0;
        for (int i = 0; i < bp->total_block_count; i++)
        {
            json_array_append_number(material, bp->blocks[i].material);
            json_array_append_number(rotation, bp->blocks[i].rotation);
            json_array_append_number(color, bp->blocks[i].color);

            char pos[100];
            snprintf(pos, 100, "%d,%d,%d",
                    bp->blocks[i].position.x,
                    bp->blocks[i].position.y,
                    bp->blocks[i].position.z
            );
            json_array_append_string(position, pos);

            uint32_t id = 0;
            if (bp->blocks[i].string_data != NULL)
            {
                counter++;
                json_array_append_string(data, (char *) bp->blocks[i].string_data->data);
                json_array_append_number(data_id, counter);
                id = counter;
            }
            char bp1_str[100];
            snprintf(bp1_str, 100, "0,0,0,%d", id);

            json_array_append_string(bp1, bp1_str);
            json_array_append_string(bp2, "0,0,0,0");
        }
        json_object_set_value(object, "BlockIds", material_val);
        json_object_set_value(object, "BLP", position_val);
        json_object_set_value(object, "BLR", rotation_val);
        json_object_set_value(object, "BCI", color_val);
        json_object_set_value(object, "BP1", bp1_val);
        json_object_set_value(object, "BP2", bp2_val);
        json_object_set_value(object, "BlockStringData", data_val);
        json_object_set_value(object, "BlockStringDataIds", data_id_val);


        json_object_set_value(output, "Blueprint", value);
    }

    serialized_string = json_serialize_to_string_pretty(joutput_val);
    output_str = bfromcstr(serialized_string);
    free(serialized_string);
    json_value_free(joutput_val);

    return output_str;
}
