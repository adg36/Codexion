/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: razevedo <razevedo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 15:34:57 by razevedo          #+#    #+#             */
/*   Updated: 2026/08/03 16:26:43 by razevedo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct coder
{
    int coder_id;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
} t_coder;

typedef struct dongle
{
    int dongle_id;
    int dongle_cooldown;
} t_dongle;

char	    **get_args(int argc, char **argv);
int		    are_args_valid(char **args, int len);
int		    has_invalid_numbers(char **args, int len);
int		    has_invalid_scheduler(char *str);
int		    array_len(char **arr);
void    	create_coders(int number_of_coders);
long	    get_timestamp(struct timeval start);
void	    compile(struct timeval start, int time_to_compile, int taskid);
void    	debug(struct timeval start, int time_to_debug, int taskid);
void    	refactor(struct timeval start, int time_to_refactor, int taskid);

#endif
