/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 15:14:10 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/28 16:33:42 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*int	dongles_are_unavailable(struct timeval start, t_coder *coder)
{
	if ((!coder->first_dongle->is_available
		|| (!coder->first_dongle->never_used
			&& get_timestamp(start) <= coder->first_dongle->began_cooldown + coder->sim->settings.dongle_cooldown))
			|| (!coder->second_dongle->is_available
				|| (!coder->second_dongle->never_used
					&& get_timestamp(start) <= coder->second_dongle->began_cooldown + coder->sim->settings.dongle_cooldown))
					|| coder->first_dongle == coder->second_dongle)
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
	while (dongles_are_unavailable(start, coder) || !has_priority(coder))
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
	assign_dongles(coder);
	pthread_mutex_unlock(&coder->sim->mutex_dongles);
	return (1);
}*/
int	first_dongle_is_unavailable(struct timeval start, t_coder *coder)
{
	if ((!coder->first_dongle->is_available
		|| (!coder->first_dongle->never_used
			&& get_timestamp(start) <= coder->first_dongle->began_cooldown
			+ coder->sim->settings.dongle_cooldown)))
		return (1);
	return (0);
}

int	second_dongle_is_unavailable(struct timeval start, t_coder *coder)
{
	if ((!coder->second_dongle->is_available
		|| (!coder->second_dongle->never_used
			&& get_timestamp(start) <= coder->second_dongle->began_cooldown
			+ coder->sim->settings.dongle_cooldown)))
		return (1);
	return (0);
}

int	get_dongles(struct timeval start, t_coder *coder)
{
	long	time_in_ms;
	long	remaining_ms;
	int				rc;
	struct timespec	ts;

	pthread_mutex_lock(&coder->sim->mutex_dongles);
	while(first_dongle_is_unavailable(start, coder) || !has_priority(coder, coder->first_dongle))
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
	grab_dongle(coder, coder->first_dongle);
	while(second_dongle_is_unavailable(start, coder) || !has_priority(coder, coder->second_dongle))
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
	grab_dongle(coder, coder->second_dongle);
	pthread_mutex_unlock(&coder->sim->mutex_dongles);
	return (1);
}

void	grab_dongle(t_coder *coder, t_dongle *dongle)
{
	dongle->held_by = pop_left(dongle->queue);
	dongle->is_available = 0;
	dongle->never_used = 0;
	print_logs(coder->sim->start, coder, "has taken a dongle");
}

/*void	assign_dongles(t_coder *coder)
{
	coder->first_dongle->held_by = pop_left(coder->first_dongle->queue);
	coder->second_dongle->held_by = pop_left(coder->second_dongle->queue);
	coder->first_dongle->is_available = 0;
	coder->second_dongle->is_available = 0;
	coder->first_dongle->never_used = 0;
	coder->second_dongle->never_used = 0;
	print_logs(coder->sim->start, coder, "has taken a dongle");
	print_logs(coder->sim->start, coder, "has taken a dongle");
}*/

void	release_dongles(struct timeval start, t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->mutex_dongles);
	if (coder->id == coder->first_dongle->held_by && coder->id == coder->second_dongle->held_by)
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

int	has_priority(t_coder *coder, t_dongle *dongle)
{
	if (strcmp(coder->sim->settings.scheduler, "fifo") == 0)
		push(dongle->queue, coder);
	else if (strcmp(coder->sim->settings.scheduler, "edf") == 0)
		push_and_fix(dongle->queue, &coder->sim->settings, coder);
	if (first_in_line(dongle->queue))
		return (1);
	return (0);
}
