/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 16:41:09 by mapoirie          #+#    #+#             */
/*   Updated: 2023/12/05 16:47:32 by mapoirie         ###   ########.fr       */
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
void	add_qvar_lst(t_list *exp);

//----------------------- cmd_export_utils2.c -----------------------
int		find_plus(char *s);
char	*ft_strdup_noplus2(char *s);
char	*ft_strdup_noplus(char *s);


//----------------------- cmd_env.c -----------------------
void	add_to_env(t_ms *ms, char *content);
void	print_lst_env(t_list *env);
int		exec_env(t_ms *ms);

#endif