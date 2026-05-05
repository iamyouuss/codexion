/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:59:56 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/05 17:32:40 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	convert_args(char **args, t_control *control)
{
	control->config->number_of_coders = atoi(args[0]);
	control->config->time_to_burnout = atoi(args[1]);
	control->config->time_to_compile = atoi(args[2]);
	control->config->time_to_debug = atoi(args[3]);
	control->config->time_to_refactor = atoi(args[4]);
	control->config->number_of_compiles_required = atoi(args[5]);
	control->config->dongle_cooldown = atoi(args[6]);
	control->config->scheduler = args[7];
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
	if (strcmp(scheduler, "fifo") != 0 && strcmp(scheduler, "edf") != 0)
		return (1);
	return (0);
}
