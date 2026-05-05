/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 19:16:09 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/05 17:31:19 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_error(int error_nb)
{
	if (error_nb == 1)
		printf("Wrong number of arguments (8 required)");
	if (error_nb == 2)
		printf(
			"[Error type] Arguments must be 7 positive numbers and 1 string");
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
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

unsigned long	get_current_time(void)
{
	unsigned long	ms;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ms = 1000;
	return (((unsigned long)tv.tv_sec * ms) + (tv.tv_usec / ms));
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
	pthread_mutex_lock(&coder->control->print_lock);
	if (is_simulation_running(coder->control))
    {
		printf("%lu %i %s\n",
			get_current_time() - coder->control->start_time,
			coder->id,
			action);
	}
	pthread_mutex_unlock(&coder->control->print_lock);
}
