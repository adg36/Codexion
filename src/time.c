/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 13:48:39 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/25 14:03:08 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_timestamp(struct timeval start)
{
	struct timeval		end;
	long				seconds;
	long				microseconds;
	long				total_time;
	long				time_in_ms;

	gettimeofday(&end, NULL);
	seconds = end.tv_sec - start.tv_sec;
	microseconds = end.tv_usec - start.tv_usec;
	total_time = seconds * 1000000 + microseconds;
	time_in_ms = total_time / 1000;
	return (time_in_ms);
}

struct timespec	build_deadline(long remaining_ms)
{
	struct timeval	now;
	struct timespec	ts;
	long			nsec;

	if (remaining_ms < 0)
		remaining_ms = 0;
	gettimeofday(&now, NULL);
	ts.tv_sec = now.tv_sec + (remaining_ms / 1000);
	nsec = (now.tv_usec * 1000) + ((remaining_ms % 1000) * 1000000);
	ts.tv_nsec = nsec % 1000000000;
	ts.tv_sec += nsec / 1000000000;
	return (ts);
}
