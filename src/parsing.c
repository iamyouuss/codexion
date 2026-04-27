#include "codexion.h"

int check_format_args(char **args)
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
    scheduler = str[7]
    if (strcmp(scheduler, "fifo") != 0
        && strcmp(scheduler, "edf") != 0)
        return (1);
    return 0;
}

codexion_t  fill_up_struct(char **args, codexion_t *args_struct)
{
    int i; 

    i = 0;
    while (args[i])
    {

        i++;
    }
    return (args_struct)
}

int parsing(char **args)
{
    codexion_t  args_struct;

    check_format_args(args);
    args_struct = fill_up_struct(args, &args_struct);
}