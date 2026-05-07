/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iam_youuss <iam_youuss@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 21:11:23 by iam_youuss        #+#    #+#             */
/*   Updated: 2026/05/07 14:04:24 by iam_youuss       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_dongles(t_control *control)
{
	int	i;
	int	total;

	total = control->config->number_of_coders;
	control->dongles = malloc(sizeof(t_dongle) * total);
	if (!control->dongles)
		return (1);
	i = 0;
	while (i < total)
	{
		control->dongles[i].id = i;
		if (pthread_mutex_init(&control->dongles[i].dongle_lock, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

static int	create_coders(t_control *control)
{
	int		i;
	int		total;

	total = control->config->number_of_coders;
	control->coders = malloc(sizeof(t_coder) * total);
	if (!control->coders)
		return (1);
	i = 0;
	while (i < total)
	{
		control->coders[i].id = i + 1;
		control->coders[i].number_of_compiles = 0;
		control->coders[i].left_dongle = &control->dongles[i].dongle_lock;
		control->coders[i].right_dongle = &control->dongles[
			(i + 1) % total].dongle_lock;
		control->coders[i].control = control;
		if (pthread_mutex_init(&control->coders[i].compile_lock, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

static void	convert_args(char **args, t_control *control)
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

int	init_control(char **args, t_control *control)
{
	convert_args(args, control);
	if (create_dongles(control) || create_coders(control)
		|| pthread_mutex_init(&control->run_lock, NULL) != 0
		|| pthread_mutex_init(&control->print_lock, NULL) != 0)
	{
		printf("Failed to initiate simulation");
		clean_error(control);
		return (1);
	}
	control->start_time = get_current_time();
	control->is_running = 1;
	return (0);
}
