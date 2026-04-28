#include "codexion.h"

int    ft_error(int error_nb)
{
    if (error_nb == 1)
        printf("Wrong number of arguments (8 required)");
    if (error_nb == 2)
        printf("[Error type] Arguments must follow this format:\n -numbers_of_coders: int\n -time_to_burnout: int\n -time_to_compile: int\n -time_to_debug: int\n -time_to_refactor: int\n -number_of_compiles_required: int\n -dongle_cooldown: int\n -scheduler: str ('fifo' or 'edf')");
    return (1);
}

int main(int ac, char **av)
{
    codexion_t  args_struct;

    if (ac == 9)
    {
        if (parsing(&av[1]))
            return (ft_error(2));
        fill_up_struct(&av[1], &args_struct);
    }
    else
        return (ft_error(1));
}