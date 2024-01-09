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
int		exec_export(t_ms *ms, t_cmd *cmd);

//----------------------- cmd_export_utils.c -----------------------
int		error_exp(char *content);
int		error_exp_spaces(char *content);
int		has_equal(char *content);
char	*add_qvar(t_ms *ms, char *content, int i, int u);
void	add_qvar_lst(t_ms *ms, t_list *exp);

//----------------------- cmd_export_utils2.c -----------------------
int		ft_slen(char *s);
int		slen_equal(char *content);
int		find_plus(char *s);
char	*ft_strdup_noplus2(t_ms *ms, char *s);
char	*ft_strdup_noplus(t_ms *ms, char *s);

//----------------------- cmd_export_utils3.c -----------------------
int		var_exists_exp(t_ms *ms, char *content);
int		var_exists_env(t_ms *ms, char *content);
void	prefree_minishell(t_ms *ms, char *str);

//----------------------- cmd_export_replace.c -----------------------
void	replace_in_exp(t_ms *ms, char *content);
void	replace_in_env(t_ms *ms, char *content);

//----------------------- cmd_export_join_env.c -----------------------
char	*dup_after_equal(t_ms *ms, char *s, int i);
char	*ft_sjoin(t_ms *ms, char *s1, char *s2, int i);
t_list	*join_in_env2(t_ms *ms, char *cpy_ct, char *join_ct);
void	join_in_env(t_ms *ms, char *content);

//----------------------- cmd_export_join_exp.c -----------------------
char	*sjoin_noequal(t_ms *ms, char *s1, char *s2, int i);
char	*sjoin_wquote(t_ms *ms, char *s1, char *s2, int i);
t_list	*join_in_exp3(t_ms *ms, char *cpy_ct, char *join_ct);
char	*join_in_exp2(t_ms *ms, t_list *exp_tmp, char *cpy_ct);
void	join_in_exp(t_ms *ms, char *content);

//----------------------- cmd_export_add.c -----------------------
void	add_to_exp3(t_ms *ms, t_list *new, t_list *prev);
void	add_to_exp2(t_list *new, t_list *prev, t_list **cur);
void	add_to_exp(t_ms *ms, char *content);

//----------------------- cmd_env.c -----------------------
void	add_to_env(t_ms *ms, char *content);
void	print_lst_env(t_list *env);
int		exec_env(t_ms *ms, t_cmd *cmd);

//xxxxxxxxxxxxxxxxxxxxxxxxx unset xxxxxxxxxxxxxxxxxxxxxxxxxx
//----------------------- cmd_unset.c -----------------------
void	del_var_exp(t_ms *ms, char *content);
void	del_var_env(t_ms *ms, char *content);
int		exec_unset(t_ms *ms, t_cmd *cmd);

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

//xxxxxxxxxxxxxxxxxxxxxxxxx echo xxxxxxxxxxxxxxxxxxxxxxxxxx
//----------------------- cmd_echo.c -----------------------
int	handle_echo_n(t_ms *ms, char **args, int *newline, int *after_n);
int	exec_echo(t_ms *ms, t_cmd *cmd);

//xxxxxxxxxxxxxxxxxxxxxxxxx pwd xxxxxxxxxxxxxxxxxxxxxxxxxx
//----------------------- cmd_pwd.c -----------------------
int	exec_pwd(t_ms *ms, t_cmd *cmd);

//xxxxxxxxxxxxxxxxxxxxxxxxx cd xxxxxxxxxxxxxxxxxxxxxxxxxx
//----------------------- cmd_cd.c -----------------------
int	exec_cd(t_ms *ms, t_cmd *cmd);

#endif