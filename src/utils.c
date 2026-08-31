/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 08:53:45 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/31 16:08:13 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	array_len(char **arr)
{
	int	i;

	i = 0;
	while (*arr)
	{
		i++;
		arr++;
	}
	return (i);
}

void	swap(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	has_priority(t_coder *coder)
{
	if (first_in_line(coder->first_dongle->queue) == coder->id
		&& first_in_line(coder->second_dongle->queue) == coder->id)
		return (1);
	return (0);
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
