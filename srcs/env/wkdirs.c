/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wkdirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:44:11 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/16 15:27:14 by mapoirie         ###   ########.fr       */
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

void	init_oldwkdir(t_ms *ms, int i)
{
	t_list	*tmp;

	tmp = ms->env;
	if (i == 0)
	{
		while (tmp)
		{
			if (ft_strncmp("OLDPWD", tmp->content, 6) == 0)
			{
				ms->old_wkdir = ft_strdup(tmp->content + 7);
				if (!ms->old_wkdir)
					prefree_minishell(ms, NULL);// c'est verifie
			}
			tmp = tmp->next;
		}
	}
	else
	{
		ms->old_wkdir = ft_strdup(ms->wkdir);
		if (!ms->old_wkdir)
			prefree_minishell(ms, NULL);// c'est verifie
	}
}

void	init_home(t_ms *ms)
{
	t_list	*tmp;

	tmp = ms->env;
	while (tmp)
	{
		if (ft_strncmp("HOME", tmp->content, 4) == 0)
		{
			ms->home = ft_strdup(tmp->content + 5);
			if (!ms->home)
				free_minishell(ms, 1);// c'est verifier
			return ;
		}
		tmp = tmp->next;
	}
	ms->home = NULL;
}

/*
 * init_workdir
 * Saves /or creates the env vars PWD and OLDPWD in case they don't exist or are unset (useful for cd builtin)
 * Calls init_oldworkdir (for norminette purpose)
 */

void	init_workdir(t_ms *ms, int i)
{
	char	buffer[PATH_MAX];
	char	*wd;
	
	if (ms->wkdir)
		free(ms->old_wkdir);
	init_oldwkdir(ms, i);
	wd = getcwd(buffer, PATH_MAX);
	if (wd)
	{
		free(ms->wkdir);
		ms->wkdir = ft_strdup(wd);
		if (!ms->wkdir)
			prefree_minishell(ms, NULL);// c'est verifie
	}
	if (i == 0)
		init_home(ms);
	// dprintf(2, "ms->oldwkdir = %s\n", ms->old_wkdir);
	// dprintf(2, "ms->wkdir = %s\n", ms->wkdir);
	// dprintf(2, "ms->home = %s\n", ms->home);
	ms->a++;// a enlever
}
