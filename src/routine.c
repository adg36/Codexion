/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:16:39 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/05 11:51:36 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void	*routine(void *threadarg)
{
	t_coder			*coder_data;
	t_simulation	sim_data;
	int				i;
	long			time_in_ms;

	coder_data = (t_coder *) threadarg;
	gettimeofday(&start, NULL);
	i = 0;
	while (i < coder_data->number_of_compiles_required)
	{
		// lock dongles
		pthread_mutex_lock(&mutex);
		time_in_ms = get_timestamp(start);
		printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->coder_id);
		time_in_ms = get_timestamp(start);
		printf("%ld %d has taken a dongle.\n", time_in_ms, coder_data->coder_id);
		compile(start, coder_data->time_to_compile, coder_data->coder_id);

		// dongle cooldown (NOTE: it does not belong here)
		// it needs to affect only the dongle, not the coder

		// unlock dongles
		pthread_mutex_unlock(&mutex);

		// next coder may pick up the dongles

		debug(start, coder_data->time_to_debug, coder_data->coder_id);
		refactor(start, coder_data->time_to_refactor, coder_data->coder_id);
		i++;
	}
	return NULL;
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
