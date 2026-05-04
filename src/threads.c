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

t_coder	*create_coders(pthread_mutex_t *dongles, t_control *control)
{
	int		i;
	int		total;
	t_coder	*coders;
	
	total = control->number_of_coders;
	coders = malloc(sizeof(t_coder) * total);
	if (!coders)
		return NULL;
	i = 0;
	while (i < total)
	{
		coders[i].id = i + 1;
		coders[i].number_of_compiles = 0;
		coders[i].left_dongle = &dongles[i];
		coders[i].right_dongle = &dongles[(i + 1) % total];
		coders[i].control = control;
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

void	lock_dongles(t_coder *coder)
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

void *coder_routine(void *data)
{
	t_coder	*coder;
	t_control *control;
	
	coder = (t_coder *)data;
	control = coder->control;
	while (control->is_running != 0)
	{
		if (coder->number_of_compiles == control->number_of_compiles_required)
			break;
		lock_dongles(coder);
		printf("%lld %i is compiling\n", get_current_time() - control->start_time, coder->id);
		coder->last_compile = get_current_time();
		coder->number_of_compiles++;
		usleep(control->time_to_compile);
		pthread_mutex_unlock(coder->left_dongle);
		pthread_mutex_unlock(coder->right_dongle);
		printf("%lld %i is debbuging\n", get_current_time() - control->start_time, coder->id);
		usleep(control->time_to_debug);
		printf("%lld %i is refactoring\n", get_current_time() - control->start_time, coder->id);
		usleep(control->time_to_refactor);
	}
	return NULL;
}

void	*check_burnout(t_control *control, t_coder *coders)
{
	int			i;
	long long	start;
	t_coder		coder;

	i = 0;
	start = control->start_time;
	while (i < control->number_of_coders)
	{
		coder = coders[i];
		if (get_current_time() - coder.last_compile >= control->time_to_burnout)
			{
				printf("%lld %i burned out!\n", get_current_time() - start, coder.id);
				control->is_running = 0;
				return (NULL);
			}
		i++;
	}
	return (NULL);
}

int	run(t_control *control)
{
	pthread_mutex_t	*dongles;
	t_coder		*coders;
	int				i;

	dongles = create_dongles(control->number_of_coders);
	coders = create_coders(dongles, control);
	i = 0;
	control->start_time = get_current_time();
	while (i < control->number_of_coders)
	{
		coders[i].last_compile = get_current_time();
		pthread_create(&coders[i].thread_id, NULL, coder_routine, &coders[i]);
		i++;
	}
	while (control->is_running != 0)
	{
		check_burnout(control, coders);
		usleep(1000);
	}
	i = 0;
	while (i < control->number_of_coders)
	{
		pthread_join(coders[i].thread_id, NULL);
		i++;
	}
	//free_coders(control->number_of_coders, coders);
	return (0);
}
