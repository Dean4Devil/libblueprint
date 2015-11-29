#include <stdio.h>

#include "item_dictionary.h"

void serialize_item_dictionary(JSON_Object *joutput_obj)
{
    for (int i = 0; i < NUMKEYS; i++)
    {
        char buffer[4];
        snprintf(buffer, 4, "%d", item_dictionary[i].key);
        json_object_set_string(joutput_obj, buffer, (char *) item_dictionary[i].value.data);
    }
}
