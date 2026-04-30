/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:20:28 by yghergho          #+#    #+#             */
/*   Updated: 2026/04/30 16:22:40 by yghergho         ###   ########.fr       */
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
		coders[i].id = i;
		coders[i].left_dongle = &dongles[i];
		coders[i].right_dongle = &dongles[(i + 1) % total]; //if i = 4, right = (4 + 1) % 5 = 0;
		i++;
	}
	return coders;
}

void	*create_dongles(int total)
{
	int	i;
	t_dongle	*dongles;

	dongles = malloc(sizeof(t_dongle) * total);
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < total)
	{
		dongles[i].id = i;
		pthread_mutex_init(&dongles[i].dongle_id, NULL);
	}
	return (dongles);
}

void *compile(void *data)
{
	t_tool_box	*coder;
	
	coder = (t_tool_box *)data;
	pthread_mutex_lock(coder->left_dongle);
	printf("Coder %i has taken a dongle", coder->id);
	pthread_mutex_lock(coder->right_dongle);
	printf("Coder %i has taken a dongle", coder->id);
	usleep(1000);

	return (NULL);
}

void	run(t_codexion *args_struct)
{
	pthread_mutex_t	*dongles;
	t_tool_box		*coders;
	int				i;

	dongles = create_dongles(args_struct->number_of_coders);
	coders = create_coders(args_struct->number_of_coders, dongles);
	i = 0;
	while (i < args_struct->number_of_coders)
	{
		pthread_create(&coders[i].thread_id, NULL, compile, &coders[i]);
		i++;
	}
}
