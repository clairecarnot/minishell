/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:03:19 by mapoirie          #+#    #+#             */
/*   Updated: 2023/12/11 16:22:07 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../libft/libft.h"

/*
Ajouter a la liste exp, dans l'ordre ascii
*/
void	add_to_exp(t_ms *ms, char *content)
{
	char	*cpy;
	char	*cpy_content;
	t_list	*exp_tmp;
	t_list	*exp_tmp2;
	t_list	*new;

	cpy = ft_strdup_noplus(content);
	cpy_content = add_qvar(ms, cpy);
	if (has_equal(content))
		free(cpy);
	exp_tmp = ms->exp;
	exp_tmp2 = ms->exp;
	new = ft_lstnew(cpy_content);//c'est verifie
	if (!new)
	{
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
