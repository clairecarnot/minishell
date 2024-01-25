/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:03:19 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/25 11:50:40 by mapoirie         ###   ########.fr       */
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

char	*dup_before_equal(t_ms *ms, t_cmd *cmd, char *s, t_list *new)
{
	char	*dest;
	int		i;

	dest = NULL;
	dest = malloc(sizeof(char) * ft_strlen(s) + 1);// c'est verifie 2
	if (!dest)
		(ft_lstfree(&new), free_cmd(cmd), prefree_minishell(ms, NULL));
	i = 0;
	while (((char *)s)[i] && s[i] != '=')
	{
		dest[i] = ((char *)s)[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

t_list	*add_to_exp0(char *content, char *cpy, char *cpy_ct)
{
	t_list	*new;

	new = NULL;
	if (has_equal(content))
		free(cpy);
	new = ft_lstnew(cpy_ct);// c'est verifie 2
	if (!new)
		return (NULL);
	return (new);
}

/*
Ajouter a la liste exp, dans l'ordre ascii
*/
void	add_to_exp(t_ms *ms, t_cmd *cmd, char *content, char *cpy_ct)
{
	char	*cpy;
	char	*var_name;
	t_list	*cur;
	t_list	*prev;
	t_list	*new;

	cur = ms->exp;
	cpy = ft_strdup_noplus(ms, cmd, content);// c'est verifie
	cpy_ct = add_qvar(cpy, 0);// c'est verifie 2
	if (!cpy_ct)
		(free_cmd(cmd), prefree_minishell(ms, cpy));
	new = add_to_exp0(content, cpy, cpy_ct);
	if (!new)
		(free_cmd(cmd), prefree_minishell(ms, cpy_ct));// c'est verifie 2
	while (cur)
	{
		var_name = dup_before_equal(ms, cmd, cur->content, new);//c'est verifie 2
		if (ft_strncmp(var_name, content, slen_equal(content)) >= 0)
			return (free(var_name), add_to_exp2(new, prev, &ms->exp));
		free(var_name);
		prev = cur;
		cur = cur->next;
	}
	add_to_exp3(ms, new, prev);
}
