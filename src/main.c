/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iam_youuss <iam_youuss@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 19:08:48 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/06 23:28:33 by iam_youuss       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	run(t_control *control)
{
	int	i;

	i = 0;
	while (i < control->config->number_of_coders)
	{
		control->coders[i].last_compile = get_current_time();
		pthread_create(
			&control->coders[i].thread_id,
			NULL, coder_routine, &control->coders[i]);
		i++;
	}
	pthread_create(&control->monitor_id, NULL, monitor, control);
	i = 0;
	while (i < control->config->number_of_coders)
	{
		pthread_join(control->coders[i].thread_id, NULL);
		i++;
	}
	pthread_join(control->monitor_id, NULL);
	return (0);
}

int	main(int ac, char **av)
{
	t_control	control;
	t_config	config;
	
	control.config = &config;
	if (parsing(ac - 1, &av[1]))
		return (1);
	if (init_control(&av[1], &control))
		return (1);
	if (run(&control))
		return (1);
	stop_simulation(&control);
	clean_up(&control);
	return (0);
}
