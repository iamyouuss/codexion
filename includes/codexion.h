#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct codexion {
    int numbers_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char    *scheduler;
} codexion_t;

int parsing(char **args);
int	isnumeric(int c);

#endif