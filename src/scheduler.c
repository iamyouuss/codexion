/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:20:58 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/20 17:43:13 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	fifo_lock_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(&coder->right_dongle->dongle_lock);
		pthread_mutex_lock(&coder->left_dongle->dongle_lock);
		usleep(dongles_cooldown(coder) * 1000);
		print_action(coder, "has taken a dongle");
		print_action(coder, "has taken a dongle");
	}
	else
	{
		pthread_mutex_lock(&coder->left_dongle->dongle_lock);
		pthread_mutex_lock(&coder->right_dongle->dongle_lock);
		usleep(dongles_cooldown(coder) * 1000);
		print_action(coder, "has taken a dongle");
		print_action(coder, "has taken a dongle");
	}
}

void	dispatch_dongles(t_control *control)
{
	int		i;
	int		wait_list_size;
	t_coder	*current;

	wait_list_size = 0;
	while (control->heap.size > 0)
	{
		current = pop_from_heap(&control->heap);
		if (current->left_dongle->is_available
			&& current->right_dongle->is_available)
		{
			current->left_dongle->is_available = 0;
			current->right_dongle->is_available = 0;
			current->has_dongles = 1;
			pthread_cond_broadcast(&control->heap_cond);
			break ;
		}
		control->heap.wait_list[wait_list_size] = current;
		wait_list_size++;
	}
	i = -1;
	while (++i < wait_list_size)
		push_to_heap(control->heap.wait_list[i], &control->heap);
}

void	edf_lock_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->control->heap_lock);
	coder->ticket = coder->control->ticket_counter;
	coder->control->ticket_counter++;
	push_to_heap(coder, &coder->control->heap);
	dispatch_dongles(coder->control);
	while (!coder->has_dongles && coder->control->is_running)
		pthread_cond_wait(&coder->control->heap_cond,
			&coder->control->heap_lock);
	pthread_mutex_unlock(&coder->control->heap_lock);
}
