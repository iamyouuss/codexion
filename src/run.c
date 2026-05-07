/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iam_youuss <iam_youuss@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 10:51:51 by iam_youuss        #+#    #+#             */
/*   Updated: 2026/05/07 12:01:03 by iam_youuss       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int create(t_control *control)
{
	int	i;

	i = 0;
	while (i < control->config->number_of_coders)
	{
		control->coders[i].last_compile = get_current_time();
        if (pthread_create(&control->coders[i].thread_id, NULL, coder_routine, &control->coders[i]) != 0)
        {
            pthread_mutex_lock(&control->run_lock);
            control->is_running = 0;
            pthread_mutex_unlock(&control->run_lock);
            control->config->number_of_coders = i; 
            return (1);
        }
        i++;
	}
	if (pthread_create(&control->monitor_id, NULL, monitor, control) != 0)
	{
        pthread_mutex_lock(&control->run_lock);
        control->is_running = 0;
        pthread_mutex_unlock(&control->run_lock);
        return (1);
    }
	return (0);
}

static void join(t_control *control, int monitor_created)
{
	int i;

	i = 0;
	while (i < control->config->number_of_coders)
	{
		pthread_join(control->coders[i].thread_id, NULL);
		i++;
	}
	if (monitor_created)
		pthread_join(control->monitor_id, NULL);
}

int	run(t_control *control)
{
	int	monitor_created;

	monitor_created = 0;
	if (!create(control))
		monitor_created = 1;
	else
		printf("Erro: Failed to create all threads");
	join(control, monitor_created);
	return (0);
}