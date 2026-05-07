/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iam_youuss <iam_youuss@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:59:56 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/07 14:02:41 by iam_youuss       ###   ########.fr       */
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

int	parsing(int len, char **args)
{
	int		i;

	if (len != 8)
	{
		printf("Invalid number of arguments");
		return (1);
	}
	i = 0;
	while (i < len - 1)
	{
		if (!is_numeric(args[i]))
		{
			printf("Argument %d must be numeric\n", i + 1);
			return (1);
		}
		i++;
	}
	if (strcmp(args[7], "fifo") && strcmp(args[7], "edf"))
	{
		printf("Scheduler must be 'fifo' or 'edf'");
		return (1);
	}
	return (0);
}
