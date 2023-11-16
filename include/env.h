/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:37:07 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/16 18:24:37 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "lexer.h"
# include "struct.h"

//----------------------- env.c ------------------------
int	init_env(t_ms *ms, char **env);
int	is_var_in_env(t_list *env, char *var);
int	init_workdir(t_ms *ms);
int	min_len(char *s1, char *s2);
void	swap_lst(t_list **l1, t_list **l2);
int	init_exp(t_ms *ms);

#endif
