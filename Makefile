NAME = minishell

CFLAGS = -g3 -Wall -Wextra -Werror
#CFLAGS = -g3 -Wall -Wextra -Werror -fsanitize=address

INCL = -I./libft -I./include

SRCS = main.c end.c end_lst.c end_wildol.c non_interactive.c debug.c \
srcs/lexer/lexer.c srcs/lexer/lexer_init.c srcs/lexer/lexer_utils.c \
srcs/lexer/lexer_utils2.c srcs/lexer/lexer_error.c srcs/lexer/prelexer_error.c \
srcs/lexer/lexer_parse_word.c srcs/lexer/lexer_parse_word2.c \
srcs/lexer/lexer_parse_word_wil.c srcs/lexer/lexer_parse_word_dol.c \
srcs/parser/parser.c srcs/parser/parser_utils.c \
srcs/parser/handle_cmds.c srcs/parser/handle_redirs.c \
srcs/env/env.c srcs/env/noenv.c srcs/env/exp.c srcs/env/wkdirs.c \
srcs/signals/parent_sig.c srcs/signals/child_hd_sig.c \
srcs/exec/exec.c srcs/exec/clear_exec.c srcs/exec/cmd_exec.c srcs/exec/cmd_exec_bis.c \
srcs/exec/cmd_path.c \
srcs/exec/wildcards.c srcs/exec/wildcards2.c srcs/exec/wildcards_init.c\
srcs/exec/wildcards_free.c srcs/exec/wildcards_utils.c\
srcs/exec/cmd_utils1.c srcs/exec/cmd_utils2.c srcs/exec/pipe_exec.c srcs/exec/pipe_exec_bis.c\
srcs/exec/cmd_redirs.c srcs/exec/cmd_hdoc.c srcs/exec/cmd_hdname.c \
srcs/exec/expand.c srcs/exec/expand_do_dols.c srcs/exec/expand_first.c \
srcs/exec/expand_getvar.c srcs/exec/expand_hdoc.c srcs/exec/expand_redef_args.c \
srcs/exec/expand_redirs.c srcs/exec/expand_utils1.c srcs/exec/expand_utils2.c \
srcs/builtins/export/cmd_export.c srcs/builtins/export/cmd_export_utils.c \
srcs/builtins/export/cmd_export_utils2.c srcs/builtins/export/cmd_export_utils3.c \
srcs/builtins/export/cmd_export_utils4.c srcs/builtins/env/cmd_env.c \
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
