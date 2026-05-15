/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 10:51:51 by iam_youuss        #+#    #+#             */
/*   Updated: 2026/05/15 18:00:45 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_threads(t_control *control)
{
	int	i;

	i = 0;
	while (i < control->config->number_of_coders)
	{
		control->coders[i].last_compile = get_current_time();
		if (pthread_create(&control->coders[i].thread_id,
				NULL, coder_routine, &control->coders[i]) != 0)
		{
			pthread_mutex_lock(&control->run_lock);
			control->is_running = 0;
			pthread_mutex_unlock(&control->run_lock);
			control->config->number_of_coders = i;
			return (1);
		}
		i++;
	}
	return (0);
}

static int create_monitor(t_control *control)
{
    if (control->config->number_of_coders == 1)
        return (0);
    if (pthread_create(&control->monitor_id, NULL, monitor, control) != 0)
	{
		pthread_mutex_lock(&control->run_lock);
		control->is_running = 0;
		pthread_mutex_unlock(&control->run_lock);
		return (1);
	}
    return (0);
}

static void	join(t_control *control, int monitor_created)
{
	int	i;

	i = 0;
	while (i < control->config->number_of_coders)
	{
		pthread_join(control->coders[i].thread_id, NULL);
		i++;
	}
	if (monitor_created && control->config->number_of_coders > 1)
		pthread_join(control->monitor_id, NULL);
}

int	run(t_control *control)
{
	int	monitor_created;

	monitor_created = 0;
	
	if (create_threads(control) || create_monitor(control))
	{
		printf("Error: Failed to create all threads");
		join(control, monitor_created);
		clean_up(control);
		return (1);
	}
    monitor_created = 1;
	join(control, monitor_created);
	return (0);
}
