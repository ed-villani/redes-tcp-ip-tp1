#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

FILE *createOrFindFile(char *wanted_size)
{
    FILE *fp;
    char *filename = "test.dat";
    // if (access(filename, R_OK) != -1)
    // {
    //     return fopen(filename, "r");
    // }
    fp = fopen(filename, "w");
    int wanted_size_int = atoi(wanted_size);
    if (fp)
    {
        for (int i = 0; i < wanted_size_int; i++)
        {
            fprintf(fp, "%d", i);
            if (ftell(fp) == wanted_size_int)
                break;
        }
        fclose(fp);
    }

    return fopen(filename, "r");
}