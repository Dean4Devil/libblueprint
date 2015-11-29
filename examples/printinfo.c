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
		return 1;
	}

    if ((fp = fopen(filename, "r")) != NULL)
    {
        bstring file_contents = bread((bNread) fread, fp);
        fclose(fp);

        bp = malloc(sizeof(struct blueprint));
        parse_blueprint(file_contents, bp);

        bdestroy(file_contents);
    }
    else
    {
        return 1;
    }

    printf("Blueprint %s aka %s aka %s\n\n",
           bp->name->data,
           bp->Name->data,
           bp->blueprint_name->data
    );
    printf("Saved under game version: %s\n\nTotal block count: %i\n\n",
           bp->game_version->data,
           bp->total_block_count
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
        printf("  Color Nr. %d: Hex 0x%08x, RGBA %d %d %d %d\n",
               i,
               bp->color_palette[i].hex,
               bp->color_palette[i].color.red,
               bp->color_palette[i].color.green,
               bp->color_palette[i].color.blue,
               bp->color_palette[i].color.alpha
        );
    }

    printf("\nSpecial Block data:\n");
    for (int n = 0; n < bp->total_block_count; n++)
    {
        bstring string_data;
        if ((string_data = bp->blocks[n].string_data) != NULL)
            printf("Block #%d:\n%s\n\n", n, string_data->data);
        bdestroy(string_data);

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
