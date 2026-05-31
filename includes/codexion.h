/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 18:59:04 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/31 15:22:25 by yghergho         ###   ########.fr       */
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
	int				scheduler;
}	t_config;

typedef struct s_dongle
{
	int				id;
	unsigned long	available_at;
	pthread_mutex_t	dongle_lock;

	int				is_available;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread_id;

	int				number_of_compiles;
	unsigned long	last_compile;
	pthread_mutex_t	compile_lock;

	t_dongle		*left_dongle;
	t_dongle		*right_dongle;

	t_control		*control;

	int				has_dongles;
	unsigned long	ticket;
}	t_coder;

typedef struct s_heap
{
	t_coder			**array;
	t_coder			**wait_list;
	int				size;
	int				max;	
}	t_heap;

typedef struct s_control
{
	int				is_running;
	int				start;
	unsigned long	start_time;

	t_config		*config;
	t_coder			*coders;
	t_dongle		*dongles;

	pthread_t		monitor_id;
	pthread_cond_t	start_cond;
	pthread_mutex_t	start_lock;
	pthread_mutex_t	run_lock;
	pthread_mutex_t	print_lock;

	void			(*lock_dongles)(t_coder *);
	void			(*unlock_dongles)(t_coder *);
	unsigned long	ticket_counter;
	t_heap			heap;
	pthread_mutex_t	heap_lock;
	pthread_cond_t	heap_cond;
}	t_control;

//parsing
int				parsing(int len, char **args);

//utils
void			print_action(t_coder *coder, char *action);
int				is_simulation_running(t_control *control);
unsigned long	get_current_time(void);
void			clean_up(t_control *control);

//init
int				init_control(char **args, t_control *control);

//threads
int				run(t_control *control);
void			*coder_routine(void *data);

//dongles cooldown
void			set_dongles_cooldown(t_coder *coder);
unsigned long	dongles_cooldown(t_coder *coder);
void			ft_usleep(t_control *control, unsigned long time_to_wait);

//scheduler
void			fifo_lock_dongles(t_coder *coder);
void			fifo_unlock_dongles(t_coder *coder);
void			edf_lock_dongles(t_coder *coder);
void			edf_unlock_dongles(t_coder *coder);

//heap
void			push_to_heap(t_coder *coder, t_heap *heap);
t_coder			*pop_from_heap(t_heap *heap);

//monitor
void			*monitor(void *data);

#endif
