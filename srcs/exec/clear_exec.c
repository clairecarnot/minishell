/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:12:41 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:27:32 by ccarnot          ###   ########.fr       */
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
		free_tab(cmd->args);
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

void	go_garbage(int i, t_trash *trash)
{
	static t_ms		*ms;
	static t_cmd	*cmd;

	if (i == 0)
	{
		ms = trash->ms;
		cmd = trash->cmd;
		free(trash);
	}
	else
	{
		free_cmd(cmd);
		free_minishell(ms, 1);
	}
}
