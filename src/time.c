/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 13:48:39 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/08 11:35:34 by razevedo         ###   ########.fr       */
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
