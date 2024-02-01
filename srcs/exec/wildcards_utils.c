/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:30:55 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:31:11 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	*lstint_to_tab(t_list *lst)
{
	int				i;
	int				*tab_w;
	t_list			*tmp;

	i = 0;
	tmp = lst;
	tab_w = ft_calloc(ft_lstsize(tmp), sizeof(int));
	if (!tab_w)
		return (NULL);
	while (tmp)
	{
		tab_w[i] = tmp->n;
		tmp = tmp->next;
		i++;
	}
	return (tab_w);
}

int	has_asterisk(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

void	advance_in_lst(t_list **lst, int size)
{
	int	i;

	i = 0;
	while (*lst && (i < size))
	{
		*lst = (*lst)->next;
		i++;
	}
}

int	len_dchar(char **tab)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (tab[i])
			i++;
	}
	return (i);
}

char	**copy_args(char **str)
{
	int		i;
	char	**new_str;

	i = 0;
	new_str = ft_calloc(sizeof(char *), len_dchar(str) + 1);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		new_str[i] = ft_strdup(str[i]);
		i++;
	}
	new_str[i] = 0;
	return (new_str);
}
