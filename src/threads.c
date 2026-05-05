/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:20:28 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/05 18:17:21 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle	*create_dongles(int total)
{
	int			i;
	t_dongle	*dongles;

	dongles = malloc(sizeof(t_dongle) * total);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < total)
	{
		dongles[i].id = i;
		pthread_mutex_init(&dongles[i].dongle_lock, NULL);
		i++;
	}
	return (dongles);
}

t_coder	*create_coders(t_control *control)
{
	int		i;
	int		total;
	t_coder	*coders;

	total = control->config->number_of_coders;
	coders = malloc(sizeof(t_coder) * total);
	if (!coders)
		return (NULL);
	i = 0;
	while (i < total)
	{
		coders[i].id = i + 1;
		coders[i].number_of_compiles = 0;
		coders[i].left_dongle = &control->dongles[i].dongle_lock;
		coders[i].right_dongle = &control->dongles[(i + 1) % total].dongle_lock;
		coders[i].control = control;
		pthread_mutex_init(&coders[i].compile_lock, NULL);
		i++;
	}
	return (coders);
}

void	*coder_routine(void *data)
{
	t_coder		*coder;
	t_control	*control;

	coder = (t_coder *)data;
	control = coder->control;
	while (is_simulation_running(control))
	{
		lock_dongle(coder);
		print_action(coder, "is compiling");
		coder->last_compile = get_current_time();
		pthread_mutex_lock(&coder->compile_lock);
		coder->number_of_compiles++;
		pthread_mutex_unlock(&coder->compile_lock);
		usleep(control->config->time_to_compile);
		pthread_mutex_unlock(coder->left_dongle);
		pthread_mutex_unlock(coder->right_dongle);
		print_action(coder, "is debbuging");
		usleep(control->config->time_to_debug);
		print_action(coder, "is refactoring");
		usleep(control->config->time_to_refactor);
	}
	return (NULL);
}

void	init_control(t_control *control)
{
	control->dongles = create_dongles(control->config->number_of_coders);
	control->coders = create_coders(control);
	control->start_time = get_current_time();
	pthread_mutex_init(&control->run_lock, NULL);
	pthread_mutex_init(&control->print_lock, NULL);
	control->is_running = 1;
}

