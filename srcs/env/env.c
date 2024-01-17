/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:44:11 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/17 16:12:10 by mapoirie         ###   ########.fr       */
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

// void	ft_printlst(t_list *lst)// temporaire
// {
// 	t_list *tmp;

// 	tmp = lst;
// 	while (tmp)
// 	{
// 		printf("%s\n", (char *)tmp->content);
// 		tmp = tmp->next;
// 	}
// }

void	add_pwd(t_ms *ms)
{
	char	path[1024];
	char	*pwd;
	char	*wd;
	t_list	*new_var1;

	wd = getcwd(path, sizeof(path));
	if (!wd)// a verifier
		free_minishell(ms, 1);
	pwd = ft_strjoin("PWD=", wd);
	if (!pwd)
		free_minishell(ms, 1);//a verifier
	new_var1 = ft_lstnew(pwd);
	if (!new_var1)// a verifier
	{
		free(pwd);
		free_minishell(ms, 1);
	}
	ft_lstadd_back(&ms->env, new_var1);
}

void	init_env_noenv(t_ms *ms)
{
	char	*shlvl;
	char	*underscore;
	t_list	*new_var2;
	t_list	*new_var3;

	add_pwd(ms);
	shlvl = ft_strdup("SHLVL=0");
	if (!shlvl)// a verifier
		free_minishell(ms, 1);
	new_var2 = ft_lstnew(shlvl);
	if (!new_var2)// a verifier
	{
		free(shlvl);
		free_minishell(ms, 1);
	}
	ft_lstadd_back(&ms->env, new_var2);
	underscore = ft_strdup("_=/usr/bin/");
	if (!underscore)// a verifier
		free_minishell(ms, 1);
	new_var3 = ft_lstnew(underscore);
	if (!new_var3)// a verifier
	{
		free(underscore);
		free_minishell(ms, 1);
	}
	ft_lstadd_back(&ms->env, new_var3);
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
	if (!env[0] || !env[10])
		init_env_noenv(ms);
	else
	{
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
	}
	// if (isatty(0))
	// {
	// 	dprintf(2, "isatty\n");
	// 	return (0);
	// }
	if (update_shlvl(ms))
		return (ft_lstfree(&ms->env), 1);
	return (0);
}
