/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cooldown.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 21:01:00 by iam_youuss        #+#    #+#             */
/*   Updated: 2026/05/21 10:50:36 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_dongles_cooldown(t_coder *coder)
{
	coder->left_dongle->available_at = (
			get_current_time() + coder->control->config->dongle_cooldown);
	coder->right_dongle->available_at = (
			get_current_time() + coder->control->config->dongle_cooldown);
}

unsigned long	dongles_cooldown(t_coder *coder)
{
	unsigned long	now;
	unsigned long	left;
	unsigned long	right;
	unsigned long	cooldown;

	now = get_current_time();
	left = coder->left_dongle->available_at;
	right = coder->right_dongle->available_at;
	cooldown = 0;
	if (now < right || now < left)
	{
		if (left > right)
			cooldown = left - now;
		else
			cooldown = right - now;
	}
	return (cooldown);
}

void	ft_usleep(t_control *control, unsigned long time_to_wait)
{
	unsigned long	end_time;

	end_time = get_current_time() + time_to_wait;
	while (is_simulation_running(control) && get_current_time() < end_time)
		usleep(500);
}
