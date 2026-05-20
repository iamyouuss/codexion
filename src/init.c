/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 21:11:23 by iam_youuss        #+#    #+#             */
/*   Updated: 2026/05/20 20:18:49 by yghergho         ###   ########.fr       */
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
		control->dongles[i].available_at = get_current_time();
		if (control->config->scheduler)
			control->dongles[i].is_available = 1;
		else
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
		control->coders[i].left_dongle = &control->dongles[i];
		control->coders[i].right_dongle = &control->dongles[(i + 1) % total];
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
	if (!strcmp(args[7], "fifo"))
		control->config->scheduler = 0;
	else
		control->config->scheduler = 1;
}

static int	init_edf_scheduler(t_control *control)
{
	int		i;

	i = 0;
	control->heap.array = malloc(
			sizeof(t_coder *) * control->config->number_of_coders);
	control->heap.wait_list = malloc(
			sizeof(t_coder *) * control->config->number_of_coders);
	if (!control->heap.array || !control->heap.wait_list)
		return (1);
	control->heap.max = control->config->number_of_coders;
	control->heap.size = 0;
	control->ticket_counter = 1;
	if (pthread_mutex_init(&control->heap_lock, NULL) != 0)
		return (1);
	if (pthread_cond_init(&control->heap_cond, NULL) != 0)
		return (1);
	control->lock_dongles = edf_lock_dongles;
	control->unlock_dongles = edf_unlock_dongles;
	return (0);
}

int	init_control(char **args, t_control *control)
{
	convert_args(args, control);
	if (create_dongles(control) || create_coders(control)
		|| pthread_mutex_init(&control->run_lock, NULL) != 0
		|| pthread_mutex_init(&control->print_lock, NULL) != 0
		|| pthread_mutex_init(&control->start_lock, NULL) != 0
		|| pthread_cond_init(&control->start_cond, NULL) != 0
		|| (control->config->scheduler && init_edf_scheduler(control))
	)
	{
		printf("Error: Failed to initiate simulation");
		clean_up(control);
		return (1);
	}
	if (!control->config->scheduler)
	{
		control->lock_dongles = fifo_lock_dongles;
		control->unlock_dongles = fifo_unlock_dongles;
	}
	control->start_time = get_current_time();
	control->is_running = 1;
	return (0);
}
