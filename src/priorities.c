/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priorities.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 08:53:45 by razevedo          #+#    #+#             */
/*   Updated: 2026/09/04 09:14:56 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
		return (queue->arr[0]);
	}
	root = queue->arr[0];
	queue->arr[0] = queue->arr[1];
	queue->size--;
	return (root);
}

int	first_in_line(t_queue *queue)
{
	if (queue->size <= 0)
		return (0);
	return (queue->arr[0]);
}

void	set_dongles(int i, t_settings *settings,
	t_coder *coders, t_dongle *dongles)
{
	if (i == settings->number_of_coders - 1)
	{
		coders[i].first_dongle = (
				&(dongles[(i + 1) % settings->number_of_coders]));
		coders[i].second_dongle = &(dongles[i]);
	}
	else
	{
		coders[i].first_dongle = &(dongles[i]);
		coders[i].second_dongle = (
				&(dongles[(i + 1) % settings->number_of_coders]));
	}
}
