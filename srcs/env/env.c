/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:44:11 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/29 18:31:02 by mapoirie         ###   ########.fr       */
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
				return (1);// c'est verifie
			tmp->content = replace_shlvl(tmp->content, newlvl);
			if (!tmp->content)
				return (free(newlvl), 1);//c'est verifie
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

	// dprintf(2, "pwd\n");
	wd = getcwd(path, sizeof(path));
	// wd = NULL;
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

void	add_shlvl(t_ms *ms)
{
	char	*shlvl;
	t_list	*new_var2;

	shlvl = ft_strdup("SHLVL=0");
	if (!shlvl)// c'est verifie
		free_minishell(ms, 255);
	new_var2 = ft_lstnew(shlvl);
	if (!new_var2)// c'est verifie
	{
		free(shlvl);
		free_minishell(ms, 1);
	}
	ft_lstadd_back(&ms->env, new_var2);	
}

void	add_underscore(t_ms *ms)
{
	char	*underscore;
	t_list	*new_var3;

	// dprintf(2, "underscore\n");
	underscore = ft_strdup("_=/usr/bin/");
	if (!underscore)// c'est verifie
		free_minishell(ms, 1);
	new_var3 = ft_lstnew(underscore);
	if (!new_var3)// c'est verifie
	{
		free(underscore);
		free_minishell(ms, 1);
	}
	ft_lstadd_back(&ms->env, new_var3);
}

// void	init_env_noenv(t_ms *ms)// a enlever
// {
// 	char	*shlvl;
// 	char	*underscore;
// 	t_list	*new_var2;
// 	t_list	*new_var3;

// 	add_pwd(ms);
// 	shlvl = ft_strdup("SHLVL=0");
// 	// shlvl = NULL;
// 	if (!shlvl)// c'est verifie
// 		free_minishell(ms, 255);
// 	new_var2 = ft_lstnew(shlvl);
// 	if (!new_var2)// a verifier
// 	{
// 		free(shlvl);
// 		free_minishell(ms, 1);
// 	}
// 	ft_lstadd_back(&ms->env, new_var2);
// 	underscore = ft_strdup("_=/usr/bin/");
// 	if (!underscore)// a verifier
// 		free_minishell(ms, 1);
// 	new_var3 = ft_lstnew(underscore);
// 	if (!new_var3)// a verifier
// 	{
// 		free(underscore);
// 		free_minishell(ms, 1);
// 	}
// 	ft_lstadd_back(&ms->env, new_var3);
// }

int	find_in_lst(t_list *env, char *var)
{
	t_list	*tmp_env;

	tmp_env = env;
	while (tmp_env)
	{
		if (!ft_strncmp(tmp_env->content, var, ft_strlen(var)))
			return (1);
		tmp_env = tmp_env->next;
	}
	return (0);
}

void	check_noenv(t_ms *ms)
{
	if (!find_in_lst(ms->env, "PWD="))
		add_pwd(ms);
	if (!find_in_lst(ms->env, "SHLVL="))
		add_shlvl(ms);
	if (!find_in_lst(ms->env, "_="))
		add_underscore(ms);
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
	while (env && env[i])
	{
		content = ft_strdup(env[i]);
		if (!content)
			return (ft_lstfree(&ms->env), 1);// c'est verifie
		new_var = ft_lstnew(content);
		if (!new_var)
			return (free(content), ft_lstfree(&ms->env), 1);// c'est verifie
		ft_lstadd_back(&ms->env, new_var);
		i++;
	}
	check_noenv(ms);
	if (update_shlvl(ms))
		return (ft_lstfree(&ms->env), 1);
	return (0);
}
