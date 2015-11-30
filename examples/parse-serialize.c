#include <stdio.h>
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

    // bp is valid here
    bstring ser = serialize_blueprint(bp);

    printf("%s\n", ser->data);

    bdestroy(ser);

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
