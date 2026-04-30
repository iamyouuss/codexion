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

t_tool_box	*create_coders(int total, t_dongle *dongles)
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
		coders[i].left_dongle = &dongles[i].dongle_id;
		coders[i].right_dongle = &dongles[(i + 1) % total].dongle_id; //if i = 4, right = (4 + 1) % 5 = 0;
		i++;
	}
	return coders;
}

t_dongle	*create_dongles(int total)
{
	int	i;
	t_dongle	*dongles;

	dongles = malloc(sizeof(t_dongle) * total);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < total)
	{
		dongles[i].id = i + 1;
		pthread_mutex_init(&dongles[i].dongle_id, NULL);
		i++;
	}
	return dongles;
}

void *compile(void *data)
{
	t_tool_box	*coder;
	
	coder = (t_tool_box *)data;
	pthread_mutex_lock(coder->left_dongle);
	printf("Coder %i has taken a dongle\n", coder->id);
	pthread_mutex_lock(coder->right_dongle);
	printf("Coder %i has taken a dongle\n", coder->id);
	usleep(100000);

	return (NULL);
}

void	run(t_codexion *args_struct)
{
	t_dongle	*dongles;
	t_tool_box		*coders;
	int				i;

	dongles = create_dongles(args_struct->number_of_coders);
	coders = create_coders(args_struct->number_of_coders, dongles);
	i = 0;
	while (i < args_struct->number_of_coders)
	{
		pthread_create(&coders[i].thread_id, NULL, compile, &coders[i]);
		printf("Creation of thread %i [%ld]\n", coders[i].id, coders[i].thread_id);
		i++;
	}
	while (i < args_struct->number_of_coders)
	{
		pthread_join(coders[i].thread_id, NULL);
		i++;
	}
}
