# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/29 16:45:43 by ccarnot           #+#    #+#              #
#    Updated: 2023/11/13 15:24:39 by ccarnot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CFLAGS = -g3 -Wall -Wextra -Werror
#CFLAGS = -g3 -Wall -Wextra -Werror -fsanitize=address

INCL = -I./libft -I./include

SRCS = main.c end.c srcs/lexer/lexer.c srcs/lexer/lexer_init.c \
       srcs/lexer/lexer_utils.c srcs/parser/parser.c srcs/parser/parser_utils.c \
	srcs/parser/handle_cmds.c srcs/parser/handle_ops.c srcs/parser/handle_par.c \
	srcs/parser/handle_pipes.c srcs/parser/handle_redirs.c

OBJS = $(SRCS:.c=.o)

LIBFT = ./libft/libft.a

$(NAME): $(OBJS)
	make lib
	cc -o $(NAME) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline

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
