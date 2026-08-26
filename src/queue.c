/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 11:48:53 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/26 15:38:54 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_queue	*create_queue(int capacity)
{
	t_queue	*queue;

	queue = malloc(sizeof(t_queue));

	if (!queue)
	{
		fprintf(stderr, "Error: Malloc failed.");
		return (NULL);
	}
	queue->size = 0;
	queue->capacity = capacity;
	queue->array = (t_coder *)malloc(capacity * sizeof(t_coder));
	if (!queue->array)
	{
		fprintf(stderr, "Error: Malloc failed.");
		return (NULL);
	}
	return (queue);
}

void	push_and_fix(t_queue *queue, t_settings *settings, t_coder *coder)
{
	long	deadline1;
	long	deadline2;

	deadline1 = 0;
	deadline2 = 0;
	if (queue->size == queue->capacity)
	{
		fprintf(stderr, "Queue overflow\n");
		return ;
	}
	queue->size++;
	queue->array[queue->size - 1] = coder;
	if (queue->size == QUEUE_CAPACITY)
	{
		deadline1 = queue->array[0].begin_of_last_compile + settings->time_to_burnout;
		deadline2 = queue->array[1].begin_of_last_compile + settings->time_to_burnout;
		if (deadline2 < deadline1)
			swap(&queue->array[0], &queue->array[1]);
	}
}

void	push(t_queue *queue, t_coder *coder)
{
	if (queue->size == queue->capacity)
	{
		fprintf(stderr, "Queue overflow\n");
		return ;
	}
	queue->size++;
	queue->array[queue->size - 1] = coder;
}

t_coder	pop_left(t_queue *queue, t_coder *coder)
{
	t_coder	root;

	if (queue->size <= 0)
	{
		fprintf(stderr, "queue is already empty.\n");
		return ;
	}
	if (queue->size == 1)
	{
		queue->size--;
		return (queue->array[0]);
	}
	root = queue->array[0];
	queue->array[0] = queue->array[1];
	queue->size--;
	return (root);
}

t_coder	pop_right(t_queue *queue, t_coder *coder)
{
	if (queue->size <= 0)
	{
		fprintf(stderr, "queue is already empty.\n");
		return ;
	}
	if (queue->size == 1)
	{
		queue->size--;
		return (queue->array[0]);
	}
	queue->size--;
	return (queue->array[1]);
}
