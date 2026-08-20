/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 15:14:10 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/20 17:03:57 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongles_are_unavailable(struct timeval start, t_coder *coder)
{
	if ((!coder->dongle_left->is_available
		|| (!coder->dongle_left->never_used
		&& get_timestamp(start) <= coder->dongle_left->began_cooldown + coder->sim_data->settings.dongle_cooldown))
		|| (!coder->dongle_right->is_available
		|| (!coder->dongle_right->never_used
		&& get_timestamp(start) <= coder->dongle_right->began_cooldown + coder->sim_data->settings.dongle_cooldown))
		|| coder->dongle_left == coder->dongle_right)
		{
			return (1);
		}
		return (0);
}

void	get_dongles(struct timeval start, t_coder *coder)
{
	long			time_in_ms;
	long			remaining_ms;
	struct timespec	ts;
	int				rc;

	pthread_mutex_lock(&coder->sim_data->mutex_dongles);
	while (dongles_are_unavailable(start, coder))
	{
		time_in_ms = get_timestamp(start);
		remaining_ms = coder->begin_of_last_compile + coder->sim_data->settings.time_to_compile + coder->sim_data->settings.dongle_cooldown - time_in_ms;
		ts = build_deadline(remaining_ms);
		rc = pthread_cond_timedwait(&coder->sim_data->cond_dongles,
			&coder->sim_data->mutex_dongles, &ts);
		(void)rc;
	}
	coder->dongle_left->is_available = 0;
	coder->dongle_right->is_available = 0;
	coder->dongle_left->held_by = coder->id;
	coder->dongle_right->held_by = coder->id;
	coder->dongle_left->never_used = 0;
	coder->dongle_right->never_used = 0;
	time_in_ms = get_timestamp(start);
	pthread_mutex_lock(&coder->sim_data->mutex_print);
	printf("%ld %d has taken a dongle.\n", time_in_ms, coder->id);
	printf("%ld %d has taken a dongle.\n", time_in_ms, coder->id);
	pthread_mutex_unlock(&coder->sim_data->mutex_print);
	pthread_mutex_unlock(&coder->sim_data->mutex_dongles);
}

void	release_dongles(struct timeval start, t_coder *coder)
{
	pthread_mutex_lock(&coder->sim_data->mutex_dongles);
	if (coder->id == coder->dongle_left->held_by && coder->id == coder->dongle_right->held_by)
	{
		coder->dongle_left->is_available = 1;
		coder->dongle_left->held_by = 0;
		coder->dongle_right->is_available = 1;
		coder->dongle_right->held_by = 0;
		coder->dongle_left->began_cooldown = get_timestamp(start);
		coder->dongle_right->began_cooldown = get_timestamp(start);
	}
	pthread_cond_broadcast(&coder->sim_data->cond_dongles);
	pthread_mutex_unlock(&coder->sim_data->mutex_dongles);
}
