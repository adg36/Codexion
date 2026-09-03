/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 11:48:53 by razevedo          #+#    #+#             */
/*   Updated: 2026/09/03 15:22:48 by razevedo         ###   ########.fr       */
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
	queue->arr = malloc(capacity * sizeof(int));
	if (!queue->arr)
	{
		free(queue);
		fprintf(stderr, "Error: Malloc failed.");
		return (NULL);
	}
	return (queue);
}

void	pre_enqueue(t_program *simulation)
{
	int	i;

	i = 1;
	while (i < simulation->settings.number_of_coders)
	{
		if (i % 2 != 0)
		{
			pthread_mutex_lock(&simulation->mutex_dongles);
			enqueue(&simulation->coders[i - 1]);
			pthread_mutex_unlock(&simulation->mutex_dongles);
		}
		i++;
	}
}

void	enqueue(t_coder *coder)
{
	if (first_in_line(coder->first_dongle->queue) == coder->id
		|| first_in_line(coder->second_dongle->queue) == coder->id)
		return ;
	if (strcmp(coder->sim->settings.scheduler, "fifo") == 0)
	{
		enqueue_fifo(coder, coder->first_dongle->queue);
		enqueue_fifo(coder, coder->second_dongle->queue);
	}
	else if (strcmp(coder->sim->settings.scheduler, "edf") == 0)
	{
		enqueue_edf(coder->first_dongle->queue, coder->sim, coder);
		enqueue_edf(coder->second_dongle->queue, coder->sim, coder);
	}
}

void	enqueue_edf(t_queue *queue, t_program *simulation, t_coder *coder)
{
	if (queue->size == queue->capacity)
		return ;
	queue->size++;
	queue->arr[queue->size - 1] = coder->id;
	if (queue->size == QUEUE_CAPACITY)
		swap_if_needed(simulation, queue);
}

void	enqueue_fifo(t_coder *coder, t_queue *queue)
{
	if (queue->size == queue->capacity)
		return ;
	queue->size++;
	queue->arr[queue->size - 1] = coder->id;
}
