/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_replace.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:05:12 by mapoirie          #+#    #+#             */
/*   Updated: 2023/12/06 17:10:07 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"
#include "../libft/libft.h"

/*
Remplace dans la liste exp, le contenu d'une variable deja existante
*/
void	replace_in_exp(t_ms *ms, char *content)
{
	t_list	*exp_tmp;
	t_list	*exp_tmp2;
	t_list	*new;
	
	exp_tmp = ms->exp;
	exp_tmp2 = ms->exp;
	new = ft_lstnew(content);// a proteger ?
	while (exp_tmp)
	{
		if (ft_strncmp(exp_tmp->content, content, ft_strlen_equal(content)) == 0)
		{
			new->next = exp_tmp->next;
			free(exp_tmp->content);
			free(exp_tmp);
			exp_tmp2->next = new;
			return ;
		}
		exp_tmp2 = exp_tmp;
		exp_tmp = exp_tmp->next;
	}
}

void	replace_in_env(t_ms *ms, char *content)
{
	char	*cpy_content;
	t_list	*env_tmp;
	t_list	*env_tmp2;
	t_list	*new;

	cpy_content = ft_strdup(content);//ajout protec
	env_tmp = ms->env;
	env_tmp2 = ms->env;
	new = ft_lstnew(cpy_content);// a proteger ?
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->content, content, ft_strlen_equal(content)) == 0)
		{
			new->next = env_tmp->next;
			free(env_tmp->content);
			free(env_tmp);
			env_tmp2->next = new;
			return ;
		}
		env_tmp2 = env_tmp;
		env_tmp = env_tmp->next;
	}
}