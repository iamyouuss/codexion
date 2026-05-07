/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iam_youuss <iam_youuss@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 21:01:00 by iam_youuss        #+#    #+#             */
/*   Updated: 2026/05/07 22:11:16 by iam_youuss       ###   ########.fr       */
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

unsigned long	dongles_coolddown(t_coder *coder)
{
	unsigned long	now;
	unsigned long	cooldown;

	now = get_current_time();
	cooldown = 0;
	if (now < coder->right_dongle->available_at || now < coder->left_dongle->available_at)
	{
		if (coder->left_dongle->available_at > coder->right_dongle->available_at)
			cooldown = coder->left_dongle->available_at - now;
		else
			cooldown = coder->right_dongle->available_at - now;
	}
	return (cooldown);
}

void	lock_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(&coder->right_dongle->dongle_lock);
		pthread_mutex_lock(&coder->left_dongle->dongle_lock);
		usleep(dongles_coolddown(coder) * 1000);
		print_action(coder, "has taken a dongle");
		print_action(coder, "has taken a dongle");
	}
	else
	{
		pthread_mutex_lock(&coder->left_dongle->dongle_lock);
		pthread_mutex_lock(&coder->right_dongle->dongle_lock);
		usleep(dongles_coolddown(coder) * 1000);
		print_action(coder, "has taken a dongle");
		print_action(coder, "has taken a dongle");
	}
}