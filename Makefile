# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/20 15:06:44 by razevedo          #+#    #+#              #
#    Updated: 2026/09/02 14:03:30 by razevedo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -Iinclude -pthread

SRC = src/arg_checker.c \
	  src/clean_up.c \
	  src/dongles.c \
	  src/main.c \
	  src/monitor.c \
	  src/mutexes.c \
	  src/queue.c \
	  src/routine.c \
	  src/structs.c \
	  src/swap.c \
	  src/time.c \
	  src/utils.c

OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))

all: $(NAME)

debug: CFLAGS += -g
debug: re

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
