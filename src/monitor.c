/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iam_youuss <iam_youuss@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 13:12:54 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/07 17:35:56 by iam_youuss       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_control *control)
{
	int			i;

	i = 0;
	while (i < control->config->number_of_coders)
	{
		pthread_mutex_lock(&control->coders[i].compile_lock);
		if (get_current_time() - control->coders[i].last_compile
			>= control->config->time_to_burnout)
		{
			print_action(&control->coders[i], "burned out!");
			pthread_mutex_lock(&control->run_lock);
			control->is_running = 0;
			pthread_mutex_unlock(&control->run_lock);
			pthread_mutex_unlock(&control->coders[i].compile_lock);
			return (1);
		}
		pthread_mutex_unlock(&control->coders[i].compile_lock);
		i++;
	}
	return (0);
}

static int	check_nb_of_compiles(t_control *control)
{
	int	i;
	int	finished;

	i = 0;
	finished = 0;
	while (i < control->config->number_of_coders)
	{
		pthread_mutex_lock(&control->coders[i].compile_lock);
		if (control->coders[i].number_of_compiles
			>= control->config->number_of_compiles_required)
			finished++;
		pthread_mutex_unlock(&control->coders[i].compile_lock);
		i++;
	}
	if (finished == control->config->number_of_coders)
		return (1);
	return (0);
}

void	*monitor(void *data)
{
	t_control	*control;

	control = (t_control *)data;
	while (is_simulation_running(control))
	{
		if (check_burnout(control))
			break ;
		if (check_nb_of_compiles(control))
		{
			pthread_mutex_lock(&control->run_lock);
			control->is_running = 0;
			pthread_mutex_unlock(&control->run_lock);
			pthread_mutex_lock(&control->print_lock);
			printf("%lu All coders have completed their compilations\n",
				get_current_time() - control->start_time);
			pthread_mutex_unlock(&control->print_lock);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
