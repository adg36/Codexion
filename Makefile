# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: razevedo <razevedo@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/20 15:06:44 by razevedo          #+#    #+#              #
#    Updated: 2026/07/23 17:53:29 by razevedo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -Iinclude

SRC = src/main.c \
	  src/parser.c \
	  src/utils.c

OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
