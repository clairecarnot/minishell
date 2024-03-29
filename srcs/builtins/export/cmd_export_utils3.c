/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_utils3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 11:21:55 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/25 11:43:37 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"
#include "../libft/libft.h"

/*
Cherche si une variable du meme nom existe deja
Si oui la remplacer
*/
int	var_exists_exp(t_ms *ms, char *content)
{
	int		size;
	t_list	*exp_tmp;

	size = slen_equal(content);
	exp_tmp = ms->exp;
	while (exp_tmp)
	{
		if (ft_strncmp(exp_tmp->content, content, size) == 0 && \
		ft_strncmp(exp_tmp->content, content, \
		slen_equal(exp_tmp->content)) == 0)
		{
			return (1);
		}
		exp_tmp = exp_tmp->next;
	}
	return (0);
}

int	var_exists_env(t_ms *ms, char *content)
{
	int		size;
	t_list	*env_tmp;

	size = slen_equal(content);
	env_tmp = ms->env;
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->content, content, size) == 0 && \
		ft_strncmp(env_tmp->content, content, \
		slen_equal(env_tmp->content)) == 0)
			return (1);
		env_tmp = env_tmp->next;
	}
	return (0);
}

void	prefree_minishell(t_ms *ms, char *str)
{
	if (str)
		free (str);
	ms->exit_code = 255;
	free_minishell(ms, 255);
}
