/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:37:07 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/30 10:50:32 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "general.h"

//----------------------- env.c ------------------------
char	*replace_shlvl(char *old, char *new);
int		update_shlvl(t_ms *ms);
int		init_env(t_ms *ms, char **env);

//---------------------- noenv.c ------------------------
void	add_pwd(t_ms *ms);
void	add_shlvl(t_ms *ms);
void	add_underscore(t_ms *ms);
int		find_in_lst(t_list *env, char *var);
void	check_noenv(t_ms *ms);

//----------------------- exp.c ------------------------
int		min_len(char *s1, char *s2);
void	swap_lst(void **l1, void **l2);
t_list	*lst_dup(t_list *lst_tocpy);
int		init_exp(t_ms *ms);

//----------------------- wkdirs.c ------------------------
void	free_init_workdir(t_ms *ms);
int		is_var_in_env(t_list *env, char *var);
void	init_oldwkdir(t_ms *ms, int i);
void	init_home(t_ms *ms);
void	init_workdir(t_ms *ms, int i);

#endif
