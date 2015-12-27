#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "blueprint.h"
#include "bstrlib.h"

int open_file(char* filename)
{
    FILE *fp;
    struct stat filestatus;
    struct blueprint *bp;

    if(stat(filename, &filestatus) != 0)
    {
        fprintf(stderr, "File %s not found!\n", filename);
        return -1;
    }

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Could not open file!\n");
        return -1;
    }

    bstring file_contents = bread((bNread) fread, fp);
    fclose(fp);

    bp = malloc(sizeof(struct blueprint));
    parse_blueprint(file_contents, bp);

    bdestroy(file_contents);


    if (bp == NULL)
    {
        printf("Error parsing blueprint!\n");
        return -1;
    }

    printf("Blueprint ");
    if(bp->name != NULL && bp->name->mlen != -1)
        printf("name: %s ", bp->name->data);

    if(bp->Name != NULL && bp->Name->mlen != -1)
        printf("Name: %s ", bp->Name->data);

    if (bp->blueprint_name != NULL && bp->Name->mlen != -1)
        printf("blueprintName: %s", bp->blueprint_name->data);

    printf("\n\n");

    printf("Saved under game version: %s\n\nTotal block count: %i\nMain block count: %i\n",
           bp->game_version->data,
           bp->total_block_count,
           bp->main_block_count
    );
    printf("Resouce Cost:\n  Natural: %d, Metal: %d, Oil: %d, Scrap: %d, Crystal: %d\n\n",
           bp->resource_cost[0],
           bp->resource_cost[1],
           bp->resource_cost[2],
           bp->resource_cost[3],
           bp->resource_cost[4]
    );

    printf("Color Palette used for this blueprint:\n");
    for (int i = 0; i < 32; i++)
    {
        printf("  Color Nr. %d: RGBA %d %d %d %d\n",
               i,
               bp->color_palette[i].array[0],
               bp->color_palette[i].array[1],
               bp->color_palette[i].array[2],
               bp->color_palette[i].array[3]
        );
    }

    printf("\nParameter1: %s\nParameter1: %s\n", bp->parameter1->data, bp->parameter2->data);

    printf("Local Position: %d, %d, %d\n", bp->local_position[0], bp->local_position[1], bp->local_position[2]);
    printf("Local Rotation: %f, %f, %f, %f\n", bp->local_rotation[0], bp->local_rotation[1], bp->local_rotation[2], bp->local_rotation[3]);

    uint32_t block_count[374] = { 0 };

    printf("\nSpecial Block data:\n");
    for (int n = 0; n < bp->main_block_count; n++)
    {
        block_count[bp->blocks[n].material]++;

        if(bp->blocks[n].string_data != NULL)
        {
            bstring string_data;
            if ((string_data = bp->blocks[n].string_data) != NULL)
                printf("Block #%d:\n%s\n\n", n, string_data->data);
        }

    }

    printf("Blocks used in this Blueprint:\nID 'Name': Amount\n");
    for (int i=0;i<374;i++)
    {
        if (block_count[i] != 0)
        {
            bstring blockname = get_name(i);
            printf("%d '%s': %d\n", i, blockname->data, block_count[i]);
            bdestroy(blockname);
        }
    }

    if (bp->num_sc != 0)
    {
        printf("\nThis vessel contains %d subconstructables.\n", bp->num_sc);
        for (int i = 0; i < bp->num_sc; i++)
        {
            printf("  SC with %d blocks\n", bp->SCs[i].total_block_count);
        }
    }

    free_blueprint(bp);

    return 0;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage:\n %s filename\n", argv[0]);
        return 0;
    }
    int retval = open_file(argv[1]);
    return retval;
}
