#ifndef BUILT_INS_H
# define BUILT_INS_H

# include "lexer.h"
# include "parser.h"
# include "struct.h"
# include "builtins.h"
# include "exec.h"
# include <stdbool.h>

//xxxxxxxxxxxxxxxxxxxxxxxxx export xxxxxxxxxxxxxxxxxxxxxxxxxx
//----------------------- cmd_export.c -----------------------
int		add_variable(t_ms *ms, char *content);
void	print_lst_exp(t_list *exp);
int		exec_export(t_ms *ms, t_ast *node);

//----------------------- cmd_export_utils.c -----------------------
int		error_exp(char *content);
int		error_exp_spaces(char *content);
int		has_equal(char *content);
char	*add_qvar(t_ms *ms, char *content);
void	add_qvar_lst(t_ms *ms, t_list *exp);

//----------------------- cmd_export_utils2.c -----------------------
int		ft_strlen_equal(char *content);
int		find_plus(char *s);
char	*ft_strdup_noplus2(char *s);
char	*ft_strdup_noplus(char *s);

//----------------------- cmd_export_replace.c -----------------------
void	replace_in_exp(t_ms *ms, char *content);
void	replace_in_env(t_ms *ms, char *content);

//----------------------- cmd_export_dup.c -----------------------
char	*ft_strjoin_wquote(char const *s1, char const *s2);
char	*dup_after_equal(char *s);
void	dup_in_exp(t_ms *ms, char *content);
void	dup_in_env(t_ms *ms, char *content);

//----------------------- cmd_export_add.c -----------------------
void	add_to_exp(t_ms *ms, char *content);

//----------------------- cmd_env.c -----------------------
void	add_to_env(t_ms *ms, char *content);
void	print_lst_env(t_list *env);
int		exec_env(t_ms *ms);


//xxxxxxxxxxxxxxxxxxxxxxxxx exit xxxxxxxxxxxxxxxxxxxxxxxxxx
//----------------------- cmd_exit.c -----------------------
int		count_args(char **args);
void	free_exit(t_ms *ms);
long long	atoll_exit(char *args, int *error);
int		get_exit_code(char *args, int *error);
int		exec_exit(t_ms *ms, t_cmd *cmd);

//----------------------- cmd_exit_utils.c -----------------------
bool	is_whitespace(char c);
bool	is_toobig(unsigned long long n, int sign, int *error);
bool	is_notnumeric(char *args, int *error);
char	*join_strs(t_ms *ms, char *s1, char *s2);
void	exit_msg(t_ms *ms, char *cmd, char *details, char *error);

#endif