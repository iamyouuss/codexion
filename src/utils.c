/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 19:16:09 by yghergho          #+#    #+#             */
/*   Updated: 2026/04/28 19:28:27 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

int	ft_error(int error_nb)
{
	if (error_nb == 1)
		printf("Wrong number of arguments (8 required)");
	if (error_nb == 2)
		printf("[Error type] Arguments must be 7 positive numbers and 1 string");
	if (error_nb == 3)
		printf("Something went wrong");
	if (error_nb == 4)
		printf("BURNOUT!");
	return (1);
}

int	isnumeric(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0'|| str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

long long	get_current_time()
{
	struct timeval	tv;
	long long	ms;

	gettimeofday(&tv, NULL);
	ms = 1000;
	return ((long long)tv.tv_sec * ms) + (tv.tv_usec / ms);
}

void	free_coders(int total, t_coder *coders)
{
	int	i;

	i = 0;
	while (i < total)
	{
		free(&coders[i]);
		i++;
	}
	free(coders);
}

void	print_action(t_coder *coder, char *action)
{
	printf("%lld %i %s\n",
		get_current_time() - coder->control->start_time, coder->id, action);
}