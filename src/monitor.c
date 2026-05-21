/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 13:12:54 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/21 12:11:22 by yghergho         ###   ########.fr       */
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
			if (control->config->scheduler)
            {
                pthread_mutex_lock(&control->heap_lock); // On prend la clé
				pthread_cond_broadcast(&control->heap_cond); // On crie
				pthread_mutex_unlock(&control->heap_lock);
            }
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

static void	start_in_time(t_control *control)
{
	pthread_mutex_lock(&control->start_lock);
	while (!control->start)
		pthread_cond_wait(&control->start_cond,
			&control->start_lock);
	pthread_mutex_unlock(&control->start_lock);
}

void	*monitor(void *data)
{
	t_control	*control;

	control = (t_control *)data;
	start_in_time(control);
	while (is_simulation_running(control))
	{
		if (check_burnout(control))
			break ;
		if (check_nb_of_compiles(control))
		{
			pthread_mutex_lock(&control->run_lock);
			control->is_running = 0;
			pthread_mutex_unlock(&control->run_lock);
			if (control->config->scheduler)
            {
                pthread_mutex_lock(&control->heap_lock);
				pthread_cond_broadcast(&control->heap_cond);
				pthread_mutex_unlock(&control->heap_lock);
            }
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
