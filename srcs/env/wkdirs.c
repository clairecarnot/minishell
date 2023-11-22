/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wkdirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:44:11 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/20 17:13:46 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env.h"

/*
 * is_var_in_env
 * Checks if the given var is included (1) or not (0) in the given env list
 */

int	is_var_in_env(t_list *env, char *var)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strncmp(var, tmp->content, ft_strlen(var)) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	init_oldwkdir(t_ms *ms)
{
	t_list	*tmp;

	tmp = ms->env;
	if (is_var_in_env(ms->env, "OLDPWD"))
	{
		while (tmp)
		{
			if (ft_strncmp("OLDPWD", tmp->content, 6) == 0)
			{
				ms->old_wkdir = ft_strdup(tmp->content + 7);
				if (!ms->old_wkdir)
					return (free(ms->wkdir), 1);
			}
			tmp = tmp->next;
		}
	}
	else
	{
		ms->old_wkdir = ft_strdup(ms->wkdir);
		if (!ms->old_wkdir)
			return (free(ms->wkdir), 1);
	}
	return (0);
}

/*
 * init_workdir
 * Saves /or creates the env vars PWD and OLDPWD in case they don't exist or are unset (useful for cd builtin)
 * Calls init_oldworkdir (for norminette purpose)
 */

int	init_workdir(t_ms *ms)
{
	char	buffer[PATH_MAX];
	char	*wd;

	wd = getcwd(buffer, PATH_MAX);
	ms->wkdir = ft_strdup(wd);
	if (!ms->wkdir)
		return (1);
	return (init_oldwkdir(ms));
}
