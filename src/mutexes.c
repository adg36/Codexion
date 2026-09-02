/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 14:02:20 by razevedo          #+#    #+#             */
/*   Updated: 2026/09/02 14:02:42 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_mutex_cond(t_program *simulation)
{
	pthread_mutex_init(&simulation->mutex_dongles, NULL);
	pthread_mutex_init(&simulation->mutex_print, NULL);
	pthread_mutex_init(&simulation->mutex_monitor, NULL);
	pthread_mutex_init(&simulation->mutex_compiles, NULL);
	pthread_mutex_init(&simulation->mutex_sim, NULL);
	pthread_cond_init(&simulation->cond_dongles, NULL);
	pthread_cond_init(&simulation->cond_monitor, NULL);
}

void	destroy_mutex_cond(t_program *simulation)
{
	pthread_mutex_destroy(&simulation->mutex_dongles);
	pthread_mutex_destroy(&simulation->mutex_print);
	pthread_mutex_destroy(&simulation->mutex_monitor);
	pthread_mutex_destroy(&simulation->mutex_compiles);
	pthread_mutex_destroy(&simulation->mutex_sim);
	pthread_cond_destroy(&simulation->cond_dongles);
	pthread_cond_destroy(&simulation->cond_monitor);
}
