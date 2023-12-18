/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:03:19 by mapoirie          #+#    #+#             */
/*   Updated: 2023/12/18 17:43:51 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../libft/libft.h"

void	add_to_exp3(t_ms *ms, t_list *new, t_list *prev)
{
	if (prev)
	{
		new->next = prev->next;
		prev->next = new;
	}
	else
	{
		ms->exp = new;
		ms->exp->next = NULL;
	}
}

void	add_to_exp2(t_list *new, t_list *prev)
{
	new->next = prev->next;
	prev->next = new;
	return ;
}

/*
Ajouter a la liste exp, dans l'ordre ascii
*/
void	add_to_exp(t_ms *ms, char *content)
{
	char	*cpy;
	char	*cpy_content;
	t_list	*cur;
	t_list	*prev;
	t_list	*new;

	cpy = ft_strdup_noplus(ms, content);// c'est verifie
	cpy_content = add_qvar(ms, cpy, 0, 1);// c'est verifie
	if (has_equal(content))
		free(cpy);
	cur = ms->exp;
	prev = ms->exp;
	new = ft_lstnew(cpy_content);// c'est verifie
	if (!new)
		prefree_minishell(ms, cpy_content);
	while (cur)//while lst exp existe
	{
		if (ft_strncmp(cur->content, content, ft_strlen(content)) >= 0)
			return (add_to_exp2(new, prev));
		prev = cur;
		cur = cur->next;
	}
	add_to_exp3(ms, new, prev);
}
