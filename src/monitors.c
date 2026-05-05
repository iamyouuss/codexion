/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 13:12:54 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/05 17:45:57 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*check_burnout(void *data)
{
	int			i;
	t_control	*control;

	i = 0;
	control = (t_control *)data;
	while (is_simulation_running(control))
	{
		while (i < control->config->number_of_coders)
		{
			pthread_mutex_lock(&control->coders[i].compile_lock);
			if (get_current_time() - control->coders[i].last_compile
				>= control->config->time_to_burnout)
			{
				print_action(&control->coders[i], "burned out!");
				pthread_mutex_lock(&control->coders[i].control->run_lock);
				control->is_running = 0;
				pthread_mutex_unlock(&control->coders[i].control->run_lock);
				pthread_mutex_unlock(&control->coders[i].compile_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&control->coders[i].compile_lock);
			i++;
		}
	}
	return (NULL);
}

/* void	*check_nb_of_compiles()
{
	
} */