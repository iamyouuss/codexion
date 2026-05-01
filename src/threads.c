/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:20:28 by yghergho          #+#    #+#             */
/*   Updated: 2026/04/30 17:10:56 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_tool_box	*create_coders(int total, pthread_mutex_t *dongles)
{
	int			i;
	t_tool_box	*coders;
	
	coders = malloc(sizeof(t_tool_box) * total);
	if (!coders)
		return NULL;
	i = 0;
	while (i < total)
	{
		coders[i].id = i + 1;
		coders[i].left_dongle = &dongles[i];
		coders[i].right_dongle = &dongles[(i + 1) % total]; //if i = 4, right = (4 + 1) % 5 = 0;
		coders[i].last_compile = get_current_time();
		coders[i].start_time = get_current_time();
		i++;
	}
	return coders;
}

pthread_mutex_t	*create_dongles(int total)
{
	int	i;
	pthread_mutex_t	*dongles;

	dongles = malloc(sizeof(pthread_mutex_t) * total);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < total)
	{
		pthread_mutex_init(&dongles[i], NULL);
		i++;
	}
	return dongles;
}

void *coder_routine(void *data)
{
	t_tool_box	*coder;
	long long	start;
	
	coder = (t_tool_box *)data;
	start = coder-> start_time;
	while (1)
	{
		if (get_current_time() - coder->last_compile < 6)
		{
			printf("BURNOUT!");
			return (NULL);
		}
		pthread_mutex_lock(coder->left_dongle);
		printf("%lld Coder %i has taken a dongle\n", get_current_time() - start, coder->id);
		pthread_mutex_lock(coder->right_dongle);
		printf("%lld Coder %i has taken a dongle\n", get_current_time() - start, coder->id);
		printf("%lld Coder %i is compiling\n", get_current_time() - start, coder->id);
		coder->last_compile = get_current_time();
		usleep(200000000);
		pthread_mutex_unlock(coder->left_dongle);
		pthread_mutex_unlock(coder->right_dongle);
		printf("%lld Coder %i has released both dongle\n", get_current_time() - start, coder->id);
		printf("%lld Coder %i is debbuging\n", get_current_time() - start, coder->id);
		usleep(500000000);
		printf("%lld Coder %i is refactoring\n", get_current_time() - start, coder->id);
		usleep(500000000);
	}
	return (NULL);
}

int	run(t_codexion *args_struct)
{
	pthread_mutex_t	*dongles;
	t_tool_box		*coders;
	int				i;

	dongles = create_dongles(args_struct->number_of_coders);
	coders = create_coders(args_struct->number_of_coders, dongles);
	i = 0;
	while (i < args_struct->number_of_coders)
	{
		pthread_create(&coders[i].thread_id, NULL, coder_routine, &coders[i]);
		printf("Creation of thread %i [%ld]\n", coders[i].id, coders[i].thread_id);
		i++;
	}
	while (i < args_struct->number_of_coders)
	{
		pthread_join(coders[i].thread_id, NULL);
		i++;
	}
	return (0);
}
