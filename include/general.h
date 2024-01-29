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
t_ms	*init_ms_bis(t_ms *minishell, char **env);
t_ms	*init_ms(char **env);
char	*display_prompt(t_ms *ms);
void	main_bis(t_ms *minishell);

//----------------------- non_interactive.c ------------------------
void	non_interactive_mode_bis(t_ms *minishell);
int	non_interactive_mode(t_ms *minishell, char **env);
void	ft_isatty(t_ms *minishell, char **env);

//----------------------- end_lst.c ------------------------
void	ft_intlstfree(t_list **lst);
void	ft_lstfree(t_list **lst);
void	token_lst_free(t_token **lst);
void	redirs_free(t_redirs **lst);

//----------------------- end_wildol.c ------------------------
void	wil_free(t_wil **wil);
void	dol_free(t_dol **dol);
void	free_wil_dol(t_ms *ms);

//----------------------- end.c ------------------------
void	prefree_minishell_cmd(t_ms *ms, t_cmd *cmd);
void	free_root_ast(t_ast *root);
void	free_minishell_bis(t_ms *ms, int exit_status, int exit_code);
void	free_minishell(t_ms *minishell, int exit_status);

//----------------------- debug.c ------------------------
char	*tok_to_str(t_token *token);
char	*node_to_str(t_ast *node);
void	print_token_lst(t_token *token);
void	print_redirs(t_redirs *args_enter);
void	print_lst(t_list *args_enter);
void	visit_node(t_ast *root);
void	print_tree(t_ast *root, int space);

#endif
