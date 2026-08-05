/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:16:39 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/05 15:12:06 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void	*routine(void *threadarg)
{
	t_simulation	sim_data;
	int				i;
	long			time_in_ms;

	sim_data = *(t_simulation *) threadarg;
	i = 0;
	while (i < sim_data.settings.number_of_compiles_required)
	{
		printf("Address of the simulation: %p\n", &sim_data);
		pthread_mutex_lock(&sim_data.mutex_dongles);
		time_in_ms = get_timestamp(sim_data.start);
		printf("%ld %d has taken a dongle.\n", time_in_ms, sim_data.coders[i].coder_id);
		time_in_ms = get_timestamp(sim_data.start);
		printf("%ld %d has taken a dongle.\n", time_in_ms, sim_data.coders[i].coder_id);
		compile(sim_data.start, sim_data.settings.time_to_compile, sim_data.coders[i].coder_id);

		// dongle cooldown (NOTE: it does not belong here)
		// it needs to affect only the dongle, not the coder

		// unlock dongles
		pthread_mutex_unlock(&sim_data.mutex_dongles);

		// next coder may pick up the dongles

		debug(sim_data.start, sim_data.settings.time_to_debug, sim_data.coders->coder_id);
		refactor(sim_data.start, sim_data.settings.time_to_refactor, sim_data.coders->coder_id);
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
