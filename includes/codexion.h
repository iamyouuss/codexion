/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:59:04 by yghergho          #+#    #+#             */
/*   Updated: 2026/04/30 16:11:09 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct codexion
{
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
}	t_codexion;

typedef struct tool_box
{
	int		id;
	pthread_t	thread_id;
	pthread_mutex_t	*left_dongle;
	pthread_mutex_t	*right_dongle;
	//long	death_time;
}   t_tool_box;

typedef struct dongle
{
	int	id;
	pthread_mutex_t	dongle_id;
}	t_dongle;

int		parsing(char **args);
int		isnumeric(char *str);
void	fill_up_struct(char **args, t_codexion *args_struct);
void	run(t_codexion *args_struct);

#endif