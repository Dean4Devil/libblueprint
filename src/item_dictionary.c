#include <stdio.h>

#include "item_dictionary.h"

void serialize_item_dictionary(JSON_Object *joutput_obj)
{
    for (int i = 0; i < NUMKEYS; i++)
    {
        char buffer[4];
        snprintf(buffer, 4, "%d", item_dictionary[i].key);
        json_object_set_string(joutput_obj, buffer, (char *) item_dictionary[i].id.data);
    }
}

// Returns the name associated with the given key
// Returns NULL if the key does not exists
bstring get_name(uint32_t key)
{
    for (int i = 0; i < NUMKEYS; i++)
    {
        if (item_dictionary[i].key == key)
            return bstrcpy(&item_dictionary[i].name);
    }

    return NULL;
}
