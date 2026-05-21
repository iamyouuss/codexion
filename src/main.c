/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 19:08:48 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/21 10:58:10 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_control	control;
	t_config	config;

	memset(&control, 0, sizeof(t_control));
	memset(&config, 0, sizeof(t_config));
	control.config = &config;
	if (parsing(ac - 1, &av[1]))
		return (1);
	if (init_control(&av[1], &control))
		return (1);
	if (run(&control))
		return (1);
	clean_up(&control);
	return (0);
}
