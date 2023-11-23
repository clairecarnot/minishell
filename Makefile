NAME = minishell

CFLAGS = -g3 -Wall -Wextra -Werror
#CFLAGS = -g3 -Wall -Wextra -Werror -fsanitize=address

INCL = -I./libft -I./include

SRCS = main.c end.c \
srcs/lexer/lexer.c srcs/lexer/lexer_init.c srcs/lexer/lexer_utils.c \
srcs/lexer/lexer_error.c srcs/lexer/prelexer_error.c \
srcs/lexer/lexer_parse_word.c srcs/lexer/lexer_error_redir.c \
srcs/parser/parser.c srcs/parser/parser_utils.c \
srcs/parser/handle_cmds.c srcs/parser/handle_ops.c srcs/parser/handle_par.c \
srcs/parser/handle_pipes.c srcs/parser/handle_redirs.c \
srcs/export/export.c \
srcs/env/env.c srcs/env/exp.c srcs/env/wkdirs.c \
srcs/signals/signals.c

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
