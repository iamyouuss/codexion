/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_lock.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iam_youuss <iam_youuss@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:42:39 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/07 15:33:48 by iam_youuss       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	lock_dongle(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(coder->right_dongle);
		print_action(coder, "has taken a dongle");
		pthread_mutex_lock(coder->left_dongle);
		print_action(coder, "has taken a dongle");
	}
	else
	{
		pthread_mutex_lock(coder->left_dongle);
		print_action(coder, "has taken a dongle");
		pthread_mutex_lock(coder->right_dongle);
		print_action(coder, "has taken a dongle");
	}
}

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
