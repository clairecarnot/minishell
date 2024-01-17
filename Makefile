NAME = minishell

CFLAGS = -g3 -Wall -Wextra -Werror
#CFLAGS = -g3 -Wall -Wextra -Werror -fsanitize=address

INCL = -I./libft -I./include

SRCS = main.c end.c \
srcs/lexer/lexer.c srcs/lexer/lexer_init.c srcs/lexer/lexer_utils.c \
srcs/lexer/lexer_utils2.c srcs/lexer/lexer_error.c srcs/lexer/prelexer_error.c \
srcs/lexer/lexer_parse_word.c srcs/lexer/lexer_parse_word2.c srcs/lexer/lexer_error_redir.c \
srcs/parser/parser.c srcs/parser/parser_utils.c \
srcs/parser/handle_cmds.c srcs/parser/handle_redirs.c \
srcs/env/env.c srcs/env/exp.c srcs/env/wkdirs.c \
srcs/signals/signals.c \
srcs/exec/exec.c srcs/exec/clear_exec.c srcs/exec/cmd_exec.c srcs/exec/cmd_path.c \
srcs/exec/cmd_utils.c srcs/exec/pipe_exec.c srcs/exec/expand.c srcs/exec/expand_first.c \
srcs/exec/expand_redirs.c \
srcs/exec/cmd_redirs.c srcs/exec/cmd_hdoc.c \
srcs/builtins/export/cmd_export.c srcs/builtins/export/cmd_export_utils.c \
srcs/builtins/export/cmd_export_utils2.c srcs/builtins/export/cmd_export_utils3.c srcs/builtins/env/cmd_env.c \
srcs/builtins/export/cmd_export_add.c srcs/builtins/export/cmd_export_replace.c \
srcs/builtins/export/cmd_export_join_env.c  srcs/builtins/export/cmd_export_join_exp.c \
srcs/builtins/unset/cmd_unset.c \
srcs/builtins/exit/cmd_exit.c srcs/builtins/exit/cmd_exit_utils.c \
srcs/builtins/echo/cmd_echo.c srcs/builtins/pwd/cmd_pwd.c \
srcs/builtins/cd/cmd_cd.c srcs/builtins/cd/cmd_cd_utils.c \
srcs/builtins/cd/cmd_cd_pwd_env.c srcs/builtins/cd/cmd_cd_pwd_exp.c 


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
