/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wkdirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:44:11 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/15 19:15:08 by mapoirie         ###   ########.fr       */
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

int	init_oldwkdir(t_ms *ms, int i)
{
	t_list	*tmp;

	tmp = ms->env;
	// if (is_var_in_env(ms->env, "OLDPWD"))
	if (i == 0)
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
	// dprintf(2, "ms->oldwkdir = %s\n", ms->old_wkdir);
	return (0);
}

/*
 * init_workdir
 * Saves /or creates the env vars PWD and OLDPWD in case they don't exist or are unset (useful for cd builtin)
 * Calls init_oldworkdir (for norminette purpose)
 */

int	init_workdir(t_ms *ms, int i)
{
	char	buffer[PATH_MAX];
	char	*wd;

	free(ms->wkdir);
	free(ms->old_wkdir);
	wd = getcwd(buffer, PATH_MAX);
	ms->wkdir = ft_strdup(wd);// a gerer quand dossier est supprime
	init_oldwkdir(ms, i);
	// dprintf(2, "ms->wkdir = %s\n", ms->wkdir);
	if (!ms->wkdir)
		return (1);
	return (0);
}
