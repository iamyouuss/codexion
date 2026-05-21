/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:59:56 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/21 11:44:39 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_numeric(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	superior_to_zero(int len, char **args)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (atoi(args[i]) < 1)
		{
			printf("Argument %i must be superior to 0\n", i);
			return (1);
		}
		i++;
	}
	return (0);
}

int	parsing(int len, char **args)
{
	int		i;

	if (len != 8)
	{
		printf("Invalid number of arguments\n");
		return (1);
	}
	i = 0;
	while (i < len - 1)
	{
		if (!is_numeric(args[i]))
		{
			printf("Argument %i must be a positive numeric value\n", i + 1);
			return (1);
		}
		i++;
	}
	if (strcmp(args[7], "fifo") && strcmp(args[7], "edf"))
	{
		printf("Scheduler must be 'fifo' or 'edf'\n");
		return (1);
	}
	if (superior_to_zero(len - 1, args))
		return (1);
	return (0);
}
