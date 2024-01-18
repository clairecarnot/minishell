/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:03:19 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/18 12:09:18 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../../../include/env.h"
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

void	add_to_exp2(t_list *new, t_list *prev, t_list **exp)
{
	if (!prev)
		ft_lstadd_front(exp, new);
	else
	{
		new->next = prev->next;
		prev->next = new;
	}
	return ;
}


char	*dup_before_equal(const char *s)
{
	char	*dest;
	int		i;

	dest = malloc(sizeof(char) * ft_strlen(s) + 1);
	if (!dest)
		return (0x0);
	i = 0;
	while (((char *)s)[i] && s[i] != '=')
	{
		dest[i] = ((char *)s)[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	add_to_exp0(char *content, char *cpy)
{
	if (has_equal(content))
		free(cpy);
}

/*
Ajouter a la liste exp, dans l'ordre ascii
*/
void	add_to_exp(t_ms *ms, t_cmd *cmd, char *content)
{
	char	*cpy;
	char	*cpy_content;
	char	*var_name;
	t_list	*cur;
	t_list	*prev;
	t_list	*new;

	(void)cmd;
	cpy = ft_strdup_noplus(ms, cmd, content);// c'est verifie
	cpy_content = add_qvar(ms, cpy, 0, 1);// c'est verifie
	add_to_exp0(content, cpy);
	cur = ms->exp;
	prev = NULL;
	new = ft_lstnew(cpy_content);// c'est verifie
	if (!new)
		prefree_minishell(ms, cpy_content);
	while (cur)//while lst exp existe
	{
		var_name = dup_before_equal(cur->content);
		if (ft_strncmp(var_name, content, slen_equal(content)) >= 0)
			return (free(var_name), add_to_exp2(new, prev, &ms->exp));
		free(var_name);
		prev = cur;
		cur = cur->next;
	}
	add_to_exp3(ms, new, prev);
}
