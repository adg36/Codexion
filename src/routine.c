/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:16:39 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/08 18:00:12 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*routine(void *threadarg)
{
	t_coder	*coder_data;
	int		i;
	long	time_in_ms;

	coder_data = (t_coder *) threadarg;
	i = 0;
	while (i < coder_data->sim_data->settings.number_of_compiles_required)
	{
		printf("Beginning of routine loop %d.\n", i);
		pthread_mutex_lock(&coder_data->sim_data->mutex_dongles);
		time_in_ms = get_timestamp(coder_data->sim_data->start);
		printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->id);
		time_in_ms = get_timestamp(coder_data->sim_data->start);
		printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->id);
		compile(coder_data->sim_data->start, coder_data->sim_data->settings.time_to_compile, coder_data->id);

		// dongle cooldown (NOTE: it does not belong here)
		// it needs to affect only the dongle, not the coder

		pthread_mutex_unlock(&coder_data->sim_data->mutex_dongles);
		debug(coder_data->sim_data->start, coder_data->sim_data->settings.time_to_debug, coder_data->id);
		refactor(coder_data->sim_data->start, coder_data->sim_data->settings.time_to_refactor, coder_data->id);
		printf("End of one routine loop %d.\n", i);
		i++;
	}
	return (NULL);
}

void	compile(struct timeval start, int time_to_compile, int taskid)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(start);
	printf("%ld %d is compiling\n", time_in_ms, taskid);
	usleep(time_to_compile * 1000);
}

void	debug(struct timeval start, int time_to_debug, int taskid)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(start);
	printf("%ld %d is debugging\n", time_in_ms, taskid);
	usleep(time_to_debug * 1000);
}

void	refactor(struct timeval start, int time_to_refactor, int taskid)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(start);
	printf("%ld %d is refactoring\n", time_in_ms, taskid);
	usleep(time_to_refactor * 1000);
}
