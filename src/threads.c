/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:20:28 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/21 10:55:02 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	compile(t_coder *coder)
{
	if (is_simulation_running(coder->control))
	{
		coder->control->lock_dongles(coder);
		if (!is_simulation_running(coder->control))
		{
			coder->control->unlock_dongles(coder);
			return (1);
		}
		pthread_mutex_lock(&coder->compile_lock);
		coder->last_compile = get_current_time();
		pthread_mutex_unlock(&coder->compile_lock);
		print_action(coder, "is compiling");
		ft_usleep(coder->control, coder->control->config->time_to_compile);
		if (is_simulation_running(coder->control))
		{
			pthread_mutex_lock(&coder->compile_lock);
			coder->number_of_compiles++;
			pthread_mutex_unlock(&coder->compile_lock);
		}
		set_dongles_cooldown(coder);
		coder->control->unlock_dongles(coder);
		return (0);
	}
	return (1);
}

static int	debug(t_coder *coder)
{
	if (is_simulation_running(coder->control))
	{
		print_action(coder, "is debuging");
		ft_usleep(coder->control, coder->control->config->time_to_debug);
		return (0);
	}
	return (1);
}

static int	refactor(t_coder *coder)
{
	if (is_simulation_running(coder->control))
	{
		print_action(coder, "is refactoring");
		ft_usleep(coder->control, coder->control->config->time_to_refactor);
		return (0);
	}
	return (1);
}

static void	start_in_time(t_coder *coder)
{
	pthread_mutex_lock(&coder->control->start_lock);
	while (!coder->control->start)
		pthread_cond_wait(&coder->control->start_cond,
			&coder->control->start_lock);
	pthread_mutex_unlock(&coder->control->start_lock);
}

void	*coder_routine(void *data)
{
	t_coder		*coder;
	t_control	*control;

	coder = (t_coder *)data;
	control = coder->control;
	if (coder->control->config->number_of_coders == 1)
	{
		usleep(control->config->time_to_burnout);
		print_action(coder, "burned out !");
		return (NULL);
	}
	start_in_time(coder);
	while (is_simulation_running(control))
	{
		if (compile(coder))
			break ;
		if (debug(coder))
			break ;
		if (refactor(coder))
			break ;
	}
	return (NULL);
}
