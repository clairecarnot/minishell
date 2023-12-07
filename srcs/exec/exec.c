/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:13:01 by ccarnot           #+#    #+#             */
/*   Updated: 2023/12/06 17:13:02 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	exec_andif(t_ast *node, t_ms *ms)
{
	if (execute(node->left, ms))
		return (1);
	if (execute(node->right, ms))
		return (1);
	return (0);
}

int	exec_orif(t_ast *node, t_ms *ms)
{
	if (execute(node->left, ms))
	{
		if (execute(node->right, ms))
			return (1);
	}
	return (0);
}

int	execute(t_ast *node, t_ms *ms)
{
	//init pipes?
	if (!node)
		return (0);
	if (node->type == CMD)
	{
		if (exec_cmd(node, ms) == 0)
			return (0);
	}
	if (node->type == AND_IF)
	{
		if (exec_andif(node, ms) == 0)
			return (0);
	}
	if (node->type == OR_IF)
	{
		if (exec_orif(node, ms) == 0)
			return (0);
	}
	if (node->type == PIPE)
	{
		if (exec_pipeline(node, ms) == 0)
			return (0);
	}
	return (1);
}

int	pre_exec(t_ms *ms)
{
	int	exit_code;
	// t_list	*tmp;
	// pid_t	pid;

	//handle save STDIN STDOUT
	//handle signals
	exit_code = execute(ms->root, ms);
	if (exit_code)
		return (1);
	// tmp = ms->pidlst;
	// if (tmp)
		// print_lst(tmp);
	// 	dprintf(2, "lst does not exist\n");
	// while (tmp && tmp->content)
	// {
	// 	dprintf(2, "avant waitpid\n");
	// 	// pid = *((pid_t *)tmp->content);
	// 	// dprintf(2, "pid = %d\n", *((pid_t *)tmp->content));
	// 	if (waitpid(*((pid_t *)tmp->content), NULL, 0) == -1)
	// 		perror("waitpid failed\n");
	// 	dprintf(2, "waitpid done\n");
	// 	// waitpid(pid, NULL, 0);
	// 	tmp = tmp->next;
	// }
	//clear exec
	return (exit_code);
}
