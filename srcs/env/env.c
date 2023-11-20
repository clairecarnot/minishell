/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 15:44:11 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/20 15:27:32 by ccarnot          ###   ########.fr       */
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
	int	i;
	int	j;

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
		concat[i+j] = new[j];
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
	int	lvl;
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
	int	i;

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

/*
 * init_workdir
 * Saves /or creates the env vars PWD and OLDPWD in case they don't exist or are unset (useful for cd builtin)
 */

int	init_workdir(t_ms *ms)
{
	char	buffer[PATH_MAX];
	char	*wd;
	t_list	*tmp;

	tmp = ms->env;
	wd = getcwd(buffer, PATH_MAX);
	ms->wkdir = ft_strdup(wd);
	if (!ms->wkdir)
		return (1);
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
 * min_len
 * Calculates and sends the mininum size between 2 chars*
 */

int	min_len(char *s1, char *s2)
{
	int	l1;
	int	l2;

	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	if (l1 > l2)
		return (l2);
	return (l1);
}

/*
 * Swaps 2 void*
 */

void	swap_lst(void **l1, void **l2)
{
	void	*tmp;

	tmp = *l1;
	*l1 = *l2;
	*l2 = tmp;
}

/*
 *lst_dup
 * Duplicates the given t_list
 */

t_list	*lst_dup(t_list *lst_tocpy)
{
	char	*newchar;
	t_list	*new;
	t_list	*tmp;
	t_list	*lst;

	new = NULL;
	tmp = NULL;
	if (!lst_tocpy)
		return (NULL);
	lst = lst_tocpy;
	while (lst)
	{
		newchar = ft_strdup(lst->content);
		if (!newchar)
			return (ft_lstfree(&new), NULL);
		tmp = ft_lstnew(newchar);
		if (!tmp)
			return (free(newchar), ft_lstfree(&new), NULL);
		ft_lstadd_back(&new, tmp);
		lst = lst->next;
	}
	return (new);
}

/*
 * init_exp
 * Creates a copy of the original env list, but sorted by ascii
 */

int	init_exp(t_ms *ms)
{
	t_list	*lst;
	t_list	*suiv;

	ms->exp = lst_dup(ms->env);
	if (!ms->exp)
		return (1);
	lst = ms->exp;
	while (lst)
	{
		suiv = lst->next;
		while (suiv)
		{
			if (ft_strncmp(lst->content, suiv->content, min_len(lst->content, suiv->content)) > 0)
				swap_lst(&lst->content, &suiv->content);
			suiv = suiv->next;
		}
		lst = lst->next;
	}
	return (0);
}
