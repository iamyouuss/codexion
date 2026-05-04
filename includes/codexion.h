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
#include <sys/time.h>

typedef struct s_control
{
	int		is_running;
	int		number_of_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
	long long	start_time;
}	t_control;

typedef struct s_coder
{
	int		id;
	int		number_of_compiles;
	long long	last_compile;
	pthread_t	thread_id;
	pthread_mutex_t	*left_dongle;
	pthread_mutex_t	*right_dongle;
	t_control	*control;
}   t_coder;

int		parsing(char **args);
int		isnumeric(char *str);
int		ft_error(int error_nb);
void	convert_args(char **args, t_control *control);
int		run(t_control *control);
long long		get_current_time();
void	free_coders(int total, t_coder *coders);
void	print_action(t_coder *coder, char *action);


#endif
