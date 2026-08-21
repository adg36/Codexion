/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 15:14:10 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/21 16:23:19 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongles_are_unavailable(struct timeval start, t_coder *coder)
{
	if ((!coder->dongle_l->is_available
		|| (!coder->dongle_l->never_used
		&& get_timestamp(start) <= coder->dongle_l->began_cooldown + coder->sim->settings.dongle_cooldown))
		|| (!coder->dongle_r->is_available
		|| (!coder->dongle_r->never_used
		&& get_timestamp(start) <= coder->dongle_r->began_cooldown + coder->sim->settings.dongle_cooldown))
		|| coder->dongle_l == coder->dongle_r)
		{
			return (1);
		}
		return (0);
}

int	get_dongles(struct timeval start, t_coder *coder)
{
	long			time_in_ms;
	long			remaining_ms;
	int				rc;
	struct timespec	ts;

	pthread_mutex_lock(&coder->sim->mutex_dongles);
	while (dongles_are_unavailable(start, coder))
	{
		time_in_ms = get_timestamp(start);
		remaining_ms = coder->begin_of_last_compile + coder->sim->settings.time_to_compile + coder->sim->settings.dongle_cooldown - time_in_ms;
		ts = build_deadline(remaining_ms);
		rc = pthread_cond_timedwait(&coder->sim->cond_dongles, &coder->sim->mutex_dongles, &ts);
		if (coder->sim->stop_simulation)
		{
			pthread_mutex_unlock(&coder->sim->mutex_dongles);
			return (0);
		}
	}
	coder->dongle_l->is_available = 0;
	coder->dongle_r->is_available = 0;
	coder->dongle_l->held_by = coder->id;
	coder->dongle_r->held_by = coder->id;
	coder->dongle_l->never_used = 0;
	coder->dongle_r->never_used = 0;
	time_in_ms = get_timestamp(start);
	pthread_mutex_lock(&coder->sim->mutex_print);
	printf("%ld %d has taken a dongle.\n", time_in_ms, coder->id);
	printf("%ld %d has taken a dongle.\n", time_in_ms, coder->id);
	pthread_mutex_unlock(&coder->sim->mutex_print);
	pthread_mutex_unlock(&coder->sim->mutex_dongles);
	return (1);
}

void	release_dongles(struct timeval start, t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->mutex_dongles);
	if (coder->id == coder->dongle_l->held_by && coder->id == coder->dongle_r->held_by)
	{
		coder->dongle_l->is_available = 1;
		coder->dongle_l->held_by = 0;
		coder->dongle_r->is_available = 1;
		coder->dongle_r->held_by = 0;
		coder->dongle_l->began_cooldown = get_timestamp(start);
		coder->dongle_r->began_cooldown = get_timestamp(start);
		pthread_cond_broadcast(&coder->sim->cond_dongles);
	}
	pthread_mutex_unlock(&coder->sim->mutex_dongles);
}
