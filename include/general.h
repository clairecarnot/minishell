#ifndef GENERAL_H
# define GENERAL_H

# include <stddef.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <errno.h>
# include <limits.h>
# include <dirent.h>
# include <stdbool.h>

# include "../libft/libft.h"
# include "struct.h"
# include "builtins.h"
# include "env.h"
# include "exec.h"
# include "lexer.h"
# include "parser.h"
# include "signals.h"

//----------------------- main.c ------------------------
char	*tok_to_str(t_token *token);
char	*node_to_str(t_ast *node);
void	print_token_lst(t_token *token);
void	print_lst(t_list *args_enter);
void	print_redirs(t_redirs *args_enter);
void	visit_node(t_ast *root);
t_ms	*init_ms(char **env);
char	*display_prompt(t_ms *ms);
int	non_interactive_mode(t_ms *minishell, char **env);

//----------------------- end.c ------------------------
void	prefree_minishell_cmd(t_ms *ms, t_cmd *cmd);
void	ft_intlstfree(t_list **lst);
void	ft_lstfree(t_list **lst);
void	token_lst_free(t_token **lst);
void	redirs_free(t_redirs **lst);
void	dol_free(t_dol **dol);
void	free_wil_dol(t_ms *ms);
void	free_root_ast(t_ast *root);
void	free_minishell(t_ms *minishell, int exit_status);

#endif
