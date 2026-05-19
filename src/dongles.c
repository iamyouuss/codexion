/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 21:01:00 by iam_youuss        #+#    #+#             */
/*   Updated: 2026/05/19 17:12:59 by yghergho         ###   ########.fr       */
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

void	fifo_lock_dongles(t_coder *coder)
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

int is_more_urgent(t_coder *a, t_coder *b)
{
    unsigned long   a_deadline;
    unsigned long   b_deadline;

    a_deadline = a->last_compile + a->control->config->time_to_burnout;
    b_deadline = b->last_compile + b->control->config->time_to_burnout;
    if (a_deadline < b_deadline)
        return (1);
    if (a_deadline == b_deadline)
    {
        if (a->ticket < b->ticket)
            return (1);
    }
    return (0);
}

void    swap_coders(t_coder **array, int index_a, int index_b)
{
    t_coder *temp;

    temp = array[index_a];
    array[index_a] = array[index_b];
    array[index_b] = temp;
}

void	edf_lock_dongles(t_coder *coder)
{
	(void)coder;
}
