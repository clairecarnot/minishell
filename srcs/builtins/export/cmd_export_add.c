/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:03:19 by mapoirie          #+#    #+#             */
/*   Updated: 2023/12/08 16:55:25 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../libft/libft.h"

/*
Ajouter a la liste exp, dans l'ordre ascii
*/
void	add_to_exp(t_ms *ms, char *content)
{
	char	*cpy_content;
	t_list	*exp_tmp;
	t_list	*exp_tmp2;
	t_list	*new;

	cpy_content = ft_strdup_noplus(content);
	
	exp_tmp = ms->exp;
	exp_tmp2 = ms->exp;
	new = ft_lstnew(cpy_content);
	if (!new)
	{
		dprintf(2, "add to exp !new\n");// leak a init_exp, lst_dup, strdup
		free(content);
		free(cpy_content);
		ms->exit_code = 134;
		free_minishell(ms, 1);
	}
	while (exp_tmp)
	{
		if (ft_strncmp(exp_tmp->content, content, ft_strlen(content)) >= 0)
		{
			new->next = exp_tmp2->next;
			exp_tmp2->next = new;
			return ;
		}
		exp_tmp2 = exp_tmp;
		exp_tmp = exp_tmp->next;
	}
	new->next = exp_tmp2->next;
	exp_tmp2->next = new;
}
