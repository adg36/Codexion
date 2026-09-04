/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 14:49:18 by razevedo          #+#    #+#             */
/*   Updated: 2026/09/04 09:02:00 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	swap_if_needed(t_program *sim, t_queue *q)
{
	long	deadline1;
	long	deadline2;

	pthread_mutex_lock(&sim->mutex_compiles);
	deadline1 = sim->coders[q->arr[0] - 1].begin_of_last_compile
		+ sim->settings.time_to_burnout;
	deadline2 = sim->coders[q->arr[1] - 1].begin_of_last_compile
		+ sim->settings.time_to_burnout;
	if (deadline2 < deadline1)
	{
		swap(&q->arr[0], &q->arr[1]);
		sim->coders[q->arr[1] - 1].loss++;
	}
	else if (deadline1 == deadline2)
	{
		if (sim->coders[q->arr[0] - 1].loss < sim->coders[q->arr[1] - 1].loss)
		{
			swap(&q->arr[0], &q->arr[1]);
			sim->coders[q->arr[1] - 1].loss++;
		}
		else if (
			sim->coders[q->arr[0] - 1].loss >= sim->coders[q->arr[1] - 1].loss)
			sim->coders[q->arr[1] - 1].loss++;
	}
	pthread_mutex_unlock(&sim->mutex_compiles);
}
