/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:53:37 by ccarnot           #+#    #+#             */
/*   Updated: 2023/12/06 18:37:59 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_INS_H
# define BUILT_INS_H

# include "lexer.h"
# include "parser.h"
# include "struct.h"
# include "exec.h"

# include <stdbool.h>

//----------------------- cmd_export.c -----------------------
void	add_to_exp(t_ms *ms, char *content);
int		add_variable(t_ms *ms, char *content);
void	print_lst_exp(t_list *exp);
int		exec_export(t_ms *ms);

//----------------------- cmd_export_utils.c -----------------------
int		error_exp(char *content);
int		error_exp_spaces(char *content);
int		has_equal(char *content);
char	*add_qvar(char *content);

//----------------------- cmd_env.c -----------------------
t_list	*add_to_env(t_ms *ms, char *content);
void	print_lst_env(t_list *env);
int		exec_env(t_ms *ms);

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