/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 19:08:48 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/05 17:49:16 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	run(t_control *control)
{
	int	i;

	i = 0;
	init_control(control);
	while (i < control->config->number_of_coders)
	{
		control->coders[i].last_compile = get_current_time();
		pthread_create(
			&control->coders[i].thread_id,
			NULL, coder_routine, &control->coders[i]);
		i++;
	}
	pthread_create(&control->monitor_id, NULL, check_burnout, &control);
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

	if (ac == 9)
	{
		if (parsing(&av[1]))
			return (ft_error(2));
		control.config = &config;
		convert_args(&av[1], &control);
	}
	else
		return (ft_error(1));
	if (run(&control))
		return (ft_error(3));
}
