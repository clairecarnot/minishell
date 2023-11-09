# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/29 16:45:43 by ccarnot           #+#    #+#              #
#    Updated: 2023/11/07 11:23:14 by mapoirie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CFLAGS = -g3 -Wall -Wextra -Werror

INCL = -I./libft -I./include

SRCS = main.c end.c print_level.c srcs/lexer/lexer.c srcs/lexer/lexer_init.c srcs/lexer/lexer_utils.c \
	srcs/parser/parser.c srcs/parser/utils.c 

OBJS = $(SRCS:.c=.o)

LIBFT = ./libft/libft.a

$(NAME): $(OBJS)
	make lib
	cc -o $(NAME) $(OBJS) $(LIBFT) -lreadline

all: $(NAME)

%.o:%.c
	cc $(INCL) $(CFLAGS) -o $@ -c $<

lib:
	make -C ./libft/ bonus

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean
	make all

.PHONY: all clean fclean re
