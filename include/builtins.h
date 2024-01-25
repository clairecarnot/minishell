#ifndef BUILT_INS_H
# define BUILT_INS_H

# include "lexer.h"
# include "parser.h"
# include "struct.h"
# include "builtins.h"
# include "exec.h"
# include <stdbool.h>

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxx export xxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//-------------------------- cmd_export.c --------------------------
void	add_variable2(t_ms *ms, t_cmd *cmd, char *ct);
void	add_variable(t_ms *ms, t_cmd *cmd, char *content);
int		asterisk_in_varct(t_ms *ms, char *content);
void	print_lst_exp(t_list *exp);
int		exec_export(t_ms *ms, t_cmd *cmd);

//----------------------- cmd_export_utils.c -----------------------
int		error_exp(t_ms *ms, char *content);
int		error_exp_spaces(t_ms *ms, char *content);
int		has_equal(char *content);
char	*add_qvar(char *content, int i);
void	add_qvar_lst(t_ms *ms, t_list *exp);

//----------------------- cmd_export_utils2.c -----------------------
int		ft_slen(char *s);
int		slen_equal(char *content);
int		find_p(char *s);
char	*ft_strdup_noplus2(t_ms *ms, t_cmd *cmd, char *s);
char	*ft_strdup_noplus(t_ms *ms, t_cmd *cmd, char *s);

//----------------------- cmd_export_utils3.c -----------------------
int		var_exists_exp(t_ms *ms, char *content);
int		var_exists_env(t_ms *ms, char *content);
void	prefree_minishell(t_ms *ms, char *str);

//----------------------- cmd_export_utils4.c -----------------------
int		message_error_exp_1(t_ms *ms, char *content);
int		message_error_exp_2(t_ms *ms, char *content);

//----------------------- cmd_export_replace.c -----------------------
char	*ft_sdup(t_ms *ms, t_cmd *cmd, char *s);
void	replace_in_exp(t_ms *ms, t_cmd *cmd, char *content);
void	replace_in_env(t_ms *ms, t_cmd *cmd, char *content);

//----------------------- cmd_export_join_env.c ----------------------
char	*dup_after_equal(t_ms *ms, t_cmd *cmd, char *s, int i);
char	*ft_sjoin(t_ms *ms, char *s1, char *s2, t_cmd *cmd);
t_list	*join_in_env2(t_ms *ms, char *cpy_ct, char *join_ct, t_cmd *cmd);
void	join_in_env(t_ms *ms, t_cmd *cmd, char *content);

//----------------------- cmd_export_join_exp.c ----------------------
char	*sjoin_noequal(char *s1, char *s2, int i);
char	*sjoin_wquote(char *s1, char *s2, int i);
t_list	*join_in_exp3(t_ms *ms, char *cpy_ct, char *join_ct, t_cmd *cmd);
char	*join_in_exp2(t_ms *ms, t_list *exp_tmp, char *cpy_ct, t_cmd *cmd);
void	join_in_exp(t_ms *ms, t_cmd *cmd, char *content);

//-------------------------- cmd_export_add.c -------------------------
void	add_to_exp3(t_ms *ms, t_list *new, t_list *prev);
void	add_to_exp2(t_list *new, t_list *prev, t_list **cur);
char	*dup_before_equal(t_ms *ms, t_cmd *cmd, char *s, t_list *new);
t_list	*add_to_exp0(char *content, char *cpy, char *cpy_ct);
void	add_to_exp(t_ms *ms, t_cmd *cmd, char *content, char *cpy_ct);

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx export xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//------------------------------ cmd_env.c -----------------------------
void	add_to_env(t_ms *ms, t_cmd *cmd, char *content);
void	print_lst_env(t_list *env);
int		exec_env(t_ms *ms, t_cmd *cmd);

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx unset xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//----------------------------- cmd_unset.c -----------------------------
void	del_var_exp(t_ms *ms, char *content);
void	del_var_env(t_ms *ms, char *content);
int		exec_unset(t_ms *ms, t_cmd *cmd);

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx exit xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//------------------------------ cmd_exit.c -----------------------------
int		count_args(char **args);
void	free_exit(t_ms *ms);
long long	atoll_exit(char *args, int *error);
int		get_exit_code(char *args, int *error);
int		exec_exit(t_ms *ms, t_cmd *cmd);

//--------------------------- cmd_exit_utils.c --------------------------
bool	is_whitespace(char c);
bool	is_toobig(unsigned long long n, int sign, int *error);
bool	is_notnumeric(char *args, int *error);
char	*join_strs(t_ms *ms, char *s1, char *s2);
void	exit_msg(t_ms *ms, char *cmd, char *details, char *error);

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx echo xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//------------------------------ cmd_echo.c -----------------------------
int		handle_echo_n(t_ms *ms, char **args, int *newline, int *after_n);
int		exec_echo(t_ms *ms, t_cmd *cmd);

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx pwd xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//-------------------------------- cmd_pwd.c ----------------------------
int		exec_pwd(t_ms *ms, t_cmd *cmd);

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx cd xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//-------------------------------- cmd_cd.c -----------------------------
int		cd_alone(t_ms *ms, t_cmd *cmd);
int		cd_dash(t_ms *ms, t_cmd *cmd);
int		cd_tilde(t_ms *ms, t_cmd *cmd);
int		cd_slash(t_ms *ms, t_cmd *cmd);
int		exec_cd(t_ms *ms, t_cmd *cmd);

//----------------------------- cmd_cd_utils.c --------------------------
char	*get_dir(t_ms *ms, t_cmd *cmd, char *var_line);
char	*getvar_env(t_ms *ms, t_cmd *cmd, char *var_name);
int		nosuchfile_cd(char *str);
int		cd_else(t_ms *ms, t_cmd *cmd);

//----------------------------- cmd_cd_pwd_exp.c -------------------------
void	replace_oldpwd_exp(t_ms *ms, t_cmd *cmd);
char	*add_qvar_pwd(t_ms *ms, t_cmd* cmd, char *content, int i);
void	preprefree_minishell(t_ms *ms, t_cmd *cmd);
void	replace_pwd_exp(t_ms *ms, t_cmd *cmd);
void	replace_pwd_env_exp(t_ms *ms, t_cmd *cmd);

//--------------------------- cmd_cd_pwd_env.c ---------------------------
void	replace_oldpwd_env(t_ms *ms, t_cmd *cmd);
void	replace_pwd_env(t_ms *ms, t_cmd *cmd);

#endif