/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yghergho <yghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:41:30 by yghergho          #+#    #+#             */
/*   Updated: 2026/05/20 20:21:02 by yghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_more_urgent(t_coder *a, t_coder *b)
{
	unsigned long	a_deadline;
	unsigned long	b_deadline;

	a_deadline = a->last_compile + a->control->config->time_to_burnout;
	b_deadline = b->last_compile + b->control->config->time_to_burnout;
	if (a_deadline < b_deadline)
		return (1);
	if (a_deadline == b_deadline)
	{
		if (a->ticket < b->ticket)
			return (1);
	}
	return (0);
}

static void	swap_coders(t_coder **array, int index_a, int index_b)
{
	t_coder	*temp;

	temp = array[index_a];
	array[index_a] = array[index_b];
	array[index_b] = temp;
}

void	push_to_heap(t_coder *coder, t_heap *heap)
{
	int	current;
	int	parent;

	if (heap->size == heap->max)
		return ;
	heap->array[heap->size] = coder;
	current = heap->size;
	heap->size++;
	while (current > 0)
	{
		parent = (current - 1) / 2;
		if (is_more_urgent(heap->array[current], heap->array[parent]))
			swap_coders(heap->array, current, parent);
		else
			break ;
		current = parent;
	}
}

void	sift_down(t_heap *heap)
{
	int	i;
	int	current;
	int	left_child;
	int	right_child;

	i = 0;
	while (i < heap->size)
	{
		current = i;
		left_child = current * 2 + 1;
		right_child = current * 2 + 2;
		if (left_child < heap->size
			&& is_more_urgent(heap->array[left_child], heap->array[current]))
			current = left_child;
		if (right_child < heap->size
			&& is_more_urgent(heap->array[right_child], heap->array[current]))
			current = right_child;
		if (current == i)
			break ;
		else
			swap_coders(heap->array, i, current);
		i = current;
	}
}

t_coder	*pop_from_heap(t_heap *heap)
{
	t_coder	*top;

	if (heap->size == 0)
		return (NULL);
	top = heap->array[0];
	heap->size--;
	heap->array[0] = heap->array[heap->size];
	sift_down(heap);
	return (top);
}
