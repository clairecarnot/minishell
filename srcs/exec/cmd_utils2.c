/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:20:14 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:20:15 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

void	save_ptrs(t_list **s1, t_list **s2, t_list **t1, t_list **t2)
{
	*t1 = *s1;
	*t2 = *s2;
}

char	**tab_cpy(t_ms *ms, char **tab)
{
	int		i;
	int		size;
	char	**cpy;

	size = 0;
	while (tab[size])
		size++;
	cpy = ft_calloc(size + 1, sizeof(char *));
	if (!cpy)
	{
		ms->exit_code = 134;
		return (NULL);
	}
	i = 0;
	while (i < size)
	{
		cpy[i] = ft_strdup(tab[i]);
		if (!cpy[i])
			return (free_tab(cpy), NULL);
		i++;
	}
	cpy[i] = NULL;
	return (cpy);
}

t_list	*ft_lstnew_int(int pid)
{
	t_list	*d;

	d = malloc(sizeof(t_list));
	if (!d)
		return (0x0);
	d->content = NULL;
	d->n = pid;
	d->next = 0x0;
	return (d);
}
