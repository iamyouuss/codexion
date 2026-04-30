/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 19:08:48 by yghergho          #+#    #+#             */
/*   Updated: 2026/04/30 16:05:21 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_error(int error_nb)
{
	if (error_nb == 1)
		printf("Wrong number of arguments (8 required)");
	if (error_nb == 2)
		printf("[Error type] Arguments must be 7 positive numbers and 1 string");
	return (1);
}

int	main(int ac, char **av)
{
	t_codexion	args_struct;

	if (ac == 9)
	{
		if (parsing(&av[1]))
			return (ft_error(2));
		fill_up_struct(&av[1], &args_struct);
	}
	else
		return (ft_error(1));
    run(&args_struct);
}
