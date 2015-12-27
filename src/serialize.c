#include "blueprint.h"
#include "parson.h"
#include "item_dictionary.h"

JSON_Object *serialize_bp_into_obj(JSON_Value *value, blueprint bp)
{
        JSON_Object *object = json_object(value);

        // FIXME: Statically set values are voiding information!
        JSON_Value *bei = json_value_init_array();
        json_object_set_value(object, "BEI", bei);
        // ENDFIXME

        json_object_set_boolean(object, "designChanged", bp->design_changed);

        json_object_set_number(object, "BlockCount", bp->main_block_count);
        json_object_set_number(object, "TotalBlockCount", bp->total_block_count);
        json_object_set_number(object, "LastAliveBlock", bp->last_alive_block);
        json_object_set_number(object, "blueprintVersion", bp->revision);
        json_object_set_number(object, "Id", bp->id);
        json_object_set_number(object, "ForceId", bp->force_id);
        json_object_set_number(object, "ItemNumber", bp->item_number);

        // TODO: Find out which one are necessary for a valid blueprint
        if (bp->blueprint_name != NULL)
            json_object_set_string(object, "blueprintName", (char *) bp->blueprint_name->data);
        else
            json_object_set_null(object, "blueprintName");
        if (bp->name != NULL)
            json_object_set_string(object, "Name", (char *) bp->name->data);
        else
            json_object_set_null(object, "Name");

        json_object_set_string(object, "GameVersion", (char *) bp->game_version->data);
        json_object_set_string(object, "Parameter1", (char *) bp->parameter1->data);
        json_object_set_string(object, "Parameter2", (char *) bp->parameter2->data);

        JSON_Value *cost_val = json_value_init_array();
        JSON_Array *cost = json_array(cost_val);
        for (int i = 0; i < 5; i++)
            json_array_append_number(cost, bp->resource_cost[i]);
        json_object_set_value(object, "ResourceCost", cost_val);

        JSON_Value *csi_val = json_value_init_array();
        JSON_Array *csi = json_array(csi_val);
        for (int i = 0; i < 40; i++)
            json_array_append_number(csi, bp->constructable_special_info[i]);
        json_object_set_value(object, "CSI", csi_val);

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

        // Local Position
        {
            char buffer[50];
            snprintf(buffer, 50, "%d,%d,%d",
                    bp->local_position[0],
                    bp->local_position[1],
                    bp->local_position[2]
            );
            json_object_set_string(object, "LocalPosition", buffer);
        }

        // Local Rotation
        {
            char buffer[50];
            snprintf(buffer, 50, "%f,%f,%f,%f",
                    bp->local_rotation[0],
                    bp->local_rotation[1],
                    bp->local_rotation[2],
                    bp->local_rotation[3]
            );
            json_object_set_string(object, "LocalRotation", buffer);
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

        {
            JSON_Value *scs = json_value_init_array();
            JSON_Array *scsa = json_value_get_array(scs);
            for (int i = 0; i < bp->num_sc; i++)
            {
                JSON_Value *sc = json_value_init_object();
                serialize_bp_into_obj(sc, &bp->SCs[i]);
                json_array_append_value(scsa, sc);
            }
            json_object_set_value(object, "SCs", scs);
        }

        JSON_Value *palette_val = json_value_init_array();
        JSON_Array *palette = json_array(palette_val);
        for (int i = 0; i < 32; i++)
        {
            double r,g,b,a;
            // Get color values in percent
            r = (double) bp->color_palette[i].array[0] / 256;
            g = (double) bp->color_palette[i].array[1] / 256;
            b = (double) bp->color_palette[i].array[2] / 256;
            a = (double) bp->color_palette[i].array[3] / 255;

            char string[20];
            snprintf(string, 20, "%0.2f,%.2f,%.2f,%.2f", r,g,b,a);
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

        for (int i = 0; i < bp->main_block_count; i++)
        {
            block_t cur = bp->blocks[i];
            json_array_append_number(material, cur.material);
            json_array_append_number(rotation, cur.rotation);
            json_array_append_number(color, cur.color);

            char pos[100];
            snprintf(pos, 100, "%d,%d,%d",
                    cur.position.x,
                    cur.position.y,
                    cur.position.z
            );
            json_array_append_string(position, pos);

            if (cur.string_data != NULL && cur.string_data->mlen != -1)
            {
                json_array_append_string(data, (char *) cur.string_data->data);
                json_array_append_number(data_id, cur.bp1[3]);
            }
            char bp1_str[50];
            snprintf(bp1_str, 50, "%f,%f,%f,%d", cur.bp1[0], cur.bp1[1], cur.bp1[2], (uint32_t) cur.bp1[3]);

            char bp2_str[50];
            snprintf(bp2_str, 50, "%d,%d,%d,%d", cur.bp2[0], cur.bp2[1], cur.bp2[2], cur.bp2[3]);

            json_array_append_string(bp1, bp1_str);
            json_array_append_string(bp2, bp2_str);
        }
        json_object_set_value(object, "BlockIds", material_val);
        json_object_set_value(object, "BLP", position_val);
        json_object_set_value(object, "BLR", rotation_val);
        json_object_set_value(object, "BCI", color_val);
        json_object_set_value(object, "BP1", bp1_val);
        json_object_set_value(object, "BP2", bp2_val);
        json_object_set_value(object, "BlockStringData", data_val);
        json_object_set_value(object, "BlockStringDataIds", data_id_val);

        return object;
}

bstring serialize_blueprint(blueprint bp)
{
    JSON_Value *joutput_val = json_value_init_object();
    JSON_Object *output = json_value_get_object(joutput_val);
    char *serialized_string = NULL;
    bstring output_str;

    // Name
    if (bp->Name != NULL)
        json_object_set_string(output, "Name", (char *) bp->Name->data);
    else
        json_object_set_null(output, "Name");

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
        serialize_bp_into_obj(value, bp);
        json_object_set_value(output, "Blueprint", value);
    }

    serialized_string = json_serialize_to_string_pretty(joutput_val);
    output_str = bfromcstr(serialized_string);
    free(serialized_string);
    json_value_free(joutput_val);

    return output_str;
}
