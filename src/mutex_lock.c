/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_lock.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:42:39 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/05 17:48:02 by yghergho         ###   ########.fr       */
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
	pthread_mutex_lock(&control->run_lock);
	if (!control->is_running)
	{
		pthread_mutex_unlock(&control->run_lock);
		return (0);
	}
	return (1);
}