/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 16:41:09 by mapoirie          #+#    #+#             */
/*   Updated: 2023/12/01 14:39:29 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "lexer.h"
# include "parser.h"
# include "struct.h"
# include "export.h"

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

#endif