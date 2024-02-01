/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:20:07 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:20:08 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h" 

int	delimitate_var(char *arg, int i, t_dol **dol)
{
	int	j;

	j = i + 1;
	while (arg[j] && arg[j] != '$' && arg[j] != '\"' && arg[j] != '\''
		&& (*dol)->c->n--)
		j++;
	return (j);
}

int	hd_delimitate_var(char *arg, int i)
{
	int	j;

	j = i + 1;
	while (arg[j] && arg[j] != '$' && arg[j] != '\"' && arg[j] != '\'')
		j++;
	return (j);
}

void	free_if(char *str)
{
	if (str)
		free(str);
}

int	tab_size(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return (0);
	while (tab[i])
		i++;
	return (i);
}

int	contains_spc(char *arg, int j, int data[5])
{
	int	i;

	i = 0;
	if (data[1])
		return (0);
	while (arg[j] && i < data[2])
	{
		if (arg[j] == ' ')
			return (1);
		j++;
		i++;
	}
	return (0);
}
