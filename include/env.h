/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:37:07 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/20 17:15:13 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "lexer.h"
# include "struct.h"

//----------------------- env.c ------------------------
char	*replace_shlvl(char *old, char *new);
int		update_shlvl(t_ms *ms);
int		init_env(t_ms *ms, char **env);
int		is_var_in_env(t_list *env, char *var);
int		init_oldwkdir(t_ms *ms);
int		init_workdir(t_ms *ms);
//
//----------------------- exp.c ------------------------
int		min_len(char *s1, char *s2);
void	swap_lst(void **l1, void **l2);
t_list	*lst_dup(t_list *lst_tocpy);
int		init_exp(t_ms *ms);

#endif
