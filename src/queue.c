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
	queue->array = malloc(capacity * sizeof(int));
	if (!queue->array)
	{
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
			enqueue(&simulation->coders[i - 1]);
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
	long	deadline1;
	long	deadline2;

	deadline1 = 0;
	deadline2 = 0;
	if (queue->size == queue->capacity)
		return ;
	queue->size++;
	queue->array[queue->size - 1] = coder->id;
	if (queue->size == QUEUE_CAPACITY)
	{
		deadline1 = simulation->coders[queue->array[0] - 1].begin_of_last_compile + simulation->settings.time_to_burnout;
		deadline2 = simulation->coders[queue->array[1] - 1].begin_of_last_compile + simulation->settings.time_to_burnout;
		if (deadline2 < deadline1)
		{
			swap(&queue->array[0], &queue->array[1]);
			simulation->coders[queue->array[1] - 1].consecutive_losses++;
		}
		else if (deadline1 == deadline2)
		{
			if (simulation->coders[queue->array[0] - 1].consecutive_losses < simulation->coders[queue->array[1] - 1].consecutive_losses)
			{
				swap(&queue->array[0], &queue->array[1]);
				simulation->coders[queue->array[1] - 1].consecutive_losses++;
			}
		}
	}
}

void	enqueue_fifo(t_coder *coder, t_queue *queue)
{
	if (queue->size == queue->capacity)
		return ;
	queue->size++;
	queue->array[queue->size - 1] = coder->id;
}

int	pop_left(t_queue *queue)
{
	int	root;

	if (queue->size <= 0)
		return (0);
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

int	first_in_line(t_queue *queue)
{
	if (queue->size <= 0)
		return (0);
	return (queue->array[0]);
}
