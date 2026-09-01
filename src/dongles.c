/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 15:14:10 by razevedo          #+#    #+#             */
/*   Updated: 2026/09/01 14:56:29 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongles_are_unavailable(struct timeval start, t_coder *coder)
{
	if (dongle_is_unavailable(start, coder, coder->first_dongle)
		|| dongle_is_unavailable(start, coder, coder->second_dongle)
		|| coder->first_dongle == coder->second_dongle)
		return (1);
	return (0);
}

int	get_dongles(struct timeval start, t_coder *coder)
{
	long			time_in_ms;
	long			remaining_ms;
	int				rc;
	struct timespec	ts;

	pthread_mutex_lock(&coder->sim->mutex_dongles);
	enqueue(coder);
	while (dongles_are_unavailable(start, coder) || !has_priority(coder))
	{
		time_in_ms = get_timestamp(start);
		remaining_ms = coder->first_dongle->began_cooldown
			+ coder->sim->settings.dongle_cooldown - time_in_ms;
		ts = build_deadline(remaining_ms);
		rc = pthread_cond_timedwait(
				&coder->sim->cond_dongles, &coder->sim->mutex_dongles, &ts);
		if (stop_simulation(coder))
		{
			pthread_mutex_unlock(&coder->sim->mutex_dongles);
			return (0);
		}
	}
	assign_dongles(coder);
	pthread_mutex_unlock(&coder->sim->mutex_dongles);
	(void)rc;
	return (1);
}

int	dongle_is_unavailable(
	struct timeval start,
	t_coder *coder,
	t_dongle *dongle)
{
	if ((!dongle->is_available
			|| (!dongle->never_used
				&& get_timestamp(start) <= dongle->began_cooldown
				+ coder->sim->settings.dongle_cooldown)))
		return (1);
	return (0);
}

void	assign_dongles(t_coder *coder)
{
	coder->first_dongle->held_by = pop_left(coder->first_dongle->queue);
	coder->second_dongle->held_by = pop_left(coder->second_dongle->queue);
	coder->first_dongle->is_available = 0;
	coder->second_dongle->is_available = 0;
	coder->first_dongle->never_used = 0;
	coder->second_dongle->never_used = 0;
	print_logs(coder->sim->start, coder, "has taken a dongle");
	print_logs(coder->sim->start, coder, "has taken a dongle");
	print_logs(coder->sim->start, coder, "is compiling");
}

void	release_dongles(struct timeval start, t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->mutex_dongles);
	if (coder->id == coder->first_dongle->held_by
		&& coder->id == coder->second_dongle->held_by)
	{
		coder->first_dongle->is_available = 1;
		coder->first_dongle->held_by = 0;
		coder->second_dongle->is_available = 1;
		coder->second_dongle->held_by = 0;
		coder->first_dongle->began_cooldown = get_timestamp(start);
		coder->second_dongle->began_cooldown = get_timestamp(start);
		pthread_cond_broadcast(&coder->sim->cond_dongles);
	}
	pthread_mutex_unlock(&coder->sim->mutex_dongles);
}
