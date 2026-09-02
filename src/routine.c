/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 10:16:39 by razevedo          #+#    #+#             */
/*   Updated: 2026/09/02 14:17:31 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*routine(void *data)
{
	t_coder	*coder;
	int		i;
	int		stop_sim;

	coder = (t_coder *)data;
	i = 0;
	while (i < coder->sim->settings.number_of_compiles_required)
	{
		pthread_mutex_lock(&coder->sim->mutex_sim);
		stop_sim = coder->sim->stop_simulation;
		pthread_mutex_unlock(&coder->sim->mutex_sim);
		if (stop_sim)
			return (NULL);
		if (coder_cycle(coder) == 0)
			return (NULL);
		i++;
	}
	return (NULL);
}

int	coder_cycle(t_coder *coder)
{
	if (!get_dongles(coder->sim->start, coder))
		return (0);
	else
	{
		compile(coder);
		if (all_compiles_completed(coder->sim))
		{
			stop_simulation(coder->sim);
			return (0);
		}
		else
		{
			release_dongles(coder->sim->start, coder);
			print_logs(coder->sim->start, coder, "is debugging");
			usleep(coder->sim->settings.time_to_debug * 1000);
			print_logs(coder->sim->start, coder, "is refactoring");
			usleep(coder->sim->settings.time_to_refactor * 1000);
		}
	}
	return (1);
}

void	compile(t_coder *coder)
{
	long	time_in_ms;

	time_in_ms = get_timestamp(coder->sim->start);
	pthread_mutex_lock(&coder->sim->mutex_compiles);
	coder->begin_of_last_compile = time_in_ms;
	pthread_mutex_unlock(&coder->sim->mutex_compiles);
	usleep(coder->sim->settings.time_to_compile * 1000);
	pthread_mutex_lock(&coder->sim->mutex_compiles);
	coder->total_compiles++;
	pthread_mutex_unlock(&coder->sim->mutex_compiles);
	pthread_mutex_lock(&coder->sim->mutex_monitor);
	pthread_cond_broadcast(&coder->sim->cond_monitor);
	pthread_mutex_unlock(&coder->sim->mutex_monitor);
}

void	print_logs(struct timeval start, t_coder *coder, char *message)
{
	long	time_in_ms;

	pthread_mutex_lock(&coder->sim->mutex_sim);
	if (!coder->sim->stop_simulation)
	{
		time_in_ms = get_timestamp(start);
		pthread_mutex_lock(&coder->sim->mutex_print);
		printf("%ld %d %s\n", time_in_ms, coder->id, message);
		pthread_mutex_unlock(&coder->sim->mutex_print);
	}
	pthread_mutex_unlock(&coder->sim->mutex_sim);
}
