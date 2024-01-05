/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:12:41 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/05 09:16:08 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd->args)
//	{
//		dprintf(2, "free args\n");
		free_tab(cmd->args);
//	}
//	if (cmd->tmp_args)
//		free_tab(cmd->tmp_args);
	if (cmd->bin_paths)
		free_tab(cmd->bin_paths);
	if (cmd->env)
		free_tab(cmd->env);
	free(cmd);
}

int	close_if(int *fd)
{
	if (*fd > 0)
	{
		if (close(*fd) < 0)
			return (-1);
		*fd = -1;
	}
	return (0);
}