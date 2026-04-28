#include "codexion.h"

void  fill_up_struct(char **args, codexion_t *args_struct)
{
    int i; 

    i = 0;
    while (args[i])
    {
        args_struct->numbers_of_coders = 0;
        printf("%i", args_struct->numbers_of_coders);
        i++;
    }
}


int parsing(char **args)
{
    int i;
    char *scheduler;

    i = 0;
    while(i < 7)
    {
        if (isnumeric(args[i]))
            return (1);
        i++;
    }
    scheduler = args[7];
    if (strcmp(scheduler, "fifo") != 0
        && strcmp(scheduler, "edf") != 0)
        return (1);
    return 0;
}

