/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:44:11 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/20 17:04:43 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env.h"

/*
 * replace_shlvl
 * Updates the char* corresponding to SHLVL by concatenating its new level number
 */

char	*replace_shlvl(char *old, char *new)
{
	char	*concat;
	int		i;
	int		j;

	i = 0;
	j = 0;
	concat = ft_calloc(ft_strlen(new) + 6 + 1, sizeof(char));
	if (!concat)
		return (NULL);
	while (old[i] && old[i] != '=')
	{
		concat[i] = old[i];
		i++;
	}
	concat[i++] = '=';
	while (new[j])
	{
		concat[i + j] = new[j];
		j++;
	}
	free(old);
	free(new);
	return (concat);
}

/*
 * update_shlvl
 * Updates the shell level (+1) by calling replace_shlvl
 */

int	update_shlvl(t_ms *ms)
{
	t_list	*tmp;
	int		lvl;
	char	*newlvl;

	tmp = ms->env;
	while (tmp)
	{
		if (ft_strncmp("SHLVL", tmp->content, 5) == 0)
		{
			lvl = ft_atoi(tmp->content + 6) + 1;
			newlvl = ft_itoa(lvl);
			if (!newlvl)
				return (1);
			tmp->content = replace_shlvl(tmp->content, newlvl);
			if (!tmp->content)
				return (1);
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

/*
 * init_env
 * Initializes and saves the env variables in a t_list
 * Updates the level of shell
 */

int	init_env(t_ms *ms, char **env)
{
	t_list	*new_var;
	char	*content;
	int		i;

	i = 0;
	content = NULL;
	ms->env = NULL;
	if (!env)
		return (1);
	while (env && env[i])
	{
		content = ft_strdup(env[i]);
		if (!content)
			return (ft_lstfree(&ms->env), 1);
		new_var = ft_lstnew(content);
		if (!new_var)
			return (free(content), ft_lstfree(&ms->env), 1);
		ft_lstadd_back(&ms->env, new_var);
		i++;
	}
	if (update_shlvl(ms))
		return (ft_lstfree(&ms->env), 1);
	return (0);
}
