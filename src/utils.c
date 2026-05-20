/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 19:16:09 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/20 17:50:07 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_simulation_running(t_control *control)
{
	int	status;

	pthread_mutex_lock(&control->run_lock);
	status = control->is_running;
	pthread_mutex_unlock(&control->run_lock);
	return (status);
}

void	print_action(t_coder *coder, char *action)
{
	pthread_mutex_lock(&coder->control->print_lock);
	if (is_simulation_running(coder->control))
	{
		printf("%lu %i %s\n",
			get_current_time() - coder->control->start_time,
			coder->id,
			action);
	}
	pthread_mutex_unlock(&coder->control->print_lock);
}

unsigned long	get_current_time(void)
{
	unsigned long	ms;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ms = 1000;
	return (((unsigned long)tv.tv_sec * ms) + (tv.tv_usec / ms));
}

void	clean_edf(t_control *control)
{
	if (control->heap.array)
	{
		free(control->heap.array);
		control->heap.array = NULL;
	}
	pthread_mutex_destroy(&control->heap_lock);
	pthread_cond_destroy(&control->heap_cond);
}

void	clean_up(t_control *control)
{
	int	i;

	if (!control)
		return ;
	if (control->dongles)
	{
		i = -1;
		if (!control->config->scheduler)
			while (++i < control->config->number_of_coders)
				pthread_mutex_destroy(&control->dongles[i].dongle_lock);
		free(control->dongles);
		control->dongles = NULL;
	}
	if (control->coders)
	{
		i = -1;
		while (++i < control->config->number_of_coders)
			pthread_mutex_destroy(&control->coders[i].compile_lock);
		free(control->coders);
		control->coders = NULL;
	}
	pthread_mutex_destroy(&control->run_lock);
	pthread_mutex_destroy(&control->print_lock);
	if (control->config->scheduler)
		clean_edf(control);
}
