#include "blueprint.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "parson.h"
#include "bstrlib.h"


void free_blueprint(blueprint bp)
{
    if (bp == NULL)
        return;

    bdestroy(bp->name);
    bdestroy(bp->blueprint_name);
    bdestroy(bp->Name);
    bdestroy(bp->game_version);
    bdestroy(bp->parameter1);
    bdestroy(bp->parameter2);

    for (int i = 0; i < bp->num_sc; i++)
        /* free_blueprint(&bp->SCs[i]); */

    for (int i = 0; i < bp->main_block_count; i++)
    {
        if (bp->blocks[i].string_data == NULL)
            continue;
        bdestroy(bp->blocks[i].string_data);
    }

    free(bp->blocks);
    free(bp);
}

