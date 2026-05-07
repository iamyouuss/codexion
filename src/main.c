/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iam_youuss <iam_youuss@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 19:08:48 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/07 15:32:28 by iam_youuss       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	clean_up(&control);
	return (0);
}
