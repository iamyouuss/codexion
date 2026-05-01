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

int	main(int ac, char **av)
{
	t_codexion	args_struct;

	if (ac == 9)
	{
		if (parsing(&av[1]))
			return (ft_error(2));
		convert_args(&av[1], &args_struct);
	}
	else
		return (ft_error(1));
    if (run(&args_struct))
		return (ft_error(3));
}
