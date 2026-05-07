/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iam_youuss <iam_youuss@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 19:16:09 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/07 12:09:06 by iam_youuss       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	clean_error(t_control *control)
{
	int i;
	
	if (control->dongles)
	{
		i = 0;
		while (i < control->config->number_of_coders)
		{
			pthread_mutex_destroy(&control->dongles[i].dongle_lock);
			free(control->dongles);
			i++;
		}
	}
}

unsigned long	get_current_time(void)
{
	unsigned long	ms;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ms = 1000;
	return (((unsigned long)tv.tv_sec * ms) + (tv.tv_usec / ms));
}

void	clean_up(t_control *control)
{
	int	i;
	
	i = 0;
	while (i < control->config->number_of_coders)
	{
		pthread_mutex_destroy(&control->coders[i].compile_lock);
		pthread_mutex_destroy(&control->dongles[i].dongle_lock);
		i++;
	}
	pthread_mutex_destroy(&control->run_lock);
	pthread_mutex_destroy(&control->print_lock);
	free(control->dongles);
	free(control->coders);
}

