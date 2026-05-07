/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iam_youuss <iam_youuss@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:59:04 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/07 10:55:57 by iam_youuss       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

struct						s_control;
typedef struct s_control	t_control;

typedef struct s_config
{
	int				number_of_coders;
	unsigned long	time_to_burnout;
	unsigned long	time_to_compile;
	unsigned long	time_to_debug;
	unsigned long	time_to_refactor;
	int				number_of_compiles_required;
	unsigned long	dongle_cooldown;
	char			*scheduler;
}	t_config;

typedef struct s_coder
{
	int				id;
	pthread_t		thread_id;

	int				number_of_compiles;
	unsigned long	last_compile;
	pthread_mutex_t	compile_lock;

	pthread_mutex_t	*left_dongle;
	pthread_mutex_t	*right_dongle;

	t_control		*control;
}	t_coder;

typedef struct s_dongle
{
	int				id;
	unsigned long	last_use;
	pthread_mutex_t	dongle_lock;
}	t_dongle;

typedef struct s_control
{
	int				is_running;
	unsigned long	start_time;

	t_config		*config;
	t_coder			*coders;
	t_dongle		*dongles;

	pthread_t		monitor_id;
	pthread_mutex_t	run_lock;
	pthread_mutex_t	print_lock;
}	t_control;

//parsing
int				parsing(int len,char **args);

//utils
void			clean_error(t_control *control);
unsigned long	get_current_time(void);
void			clean_up(t_control *control);
void			stop_simulation(t_control *control);

//init
int				init_control(char **args, t_control *control);

//threads
int				run(t_control *control);
void			*coder_routine(void *data);

//mutex_lock
void			lock_dongle(t_coder *coder);
void			print_action(t_coder *coder, char *action);
int				is_simulation_running(t_control *control);

//monitor
void			*monitor(void *data);

#endif
