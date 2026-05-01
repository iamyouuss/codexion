/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:59:56 by yghergho          #+#    #+#             */
/*   Updated: 2026/04/30 16:34:29 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	convert_args(char **args, t_codexion *args_struct)
{
	args_struct->number_of_coders = atoi(args[0]);
	args_struct->time_to_burnout = atoi(args[1]);
	args_struct->time_to_compile = atoi(args[2]);
	args_struct->time_to_debug = atoi(args[3]);
	args_struct->time_to_refactor = atoi(args[4]);
	args_struct->number_of_compiles_required = atoi(args[5]);
	args_struct->dongle_cooldown = atoi(args[6]);
	args_struct->scheduler = args[7];
}

int	parsing(char **args)
{
	int		i;
	char	*scheduler;

	i = 0;
	while (i < 7)
	{
		if (isnumeric(args[i]))
			return (1);
		i++;
	}
	scheduler = args[7];
	if (strcasecmp(scheduler, "fifo") != 0 && strcasecmp(scheduler, "edf") != 0)
		return (1);
	return (0);
}
