/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:13:01 by ccarnot           #+#    #+#             */
/*   Updated: 2023/12/13 15:54:04 by ccarnot          ###   ########.fr       */
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
	t_list	*tmp;
	// pid_t	pid;

	//handle save STDIN STDOUT
	//handle signals
	exit_code = execute(ms->root, ms);
	if (exit_code)
		return (1);
//	dprintf(2, "after exec, before waitpid\n");
	tmp = ms->pidlst;
	while (tmp)
	{
		waitpid(tmp->n, NULL, 0);
		tmp = tmp->next;
	}
//	dprintf(2, "after exec, after waitpid\n");
	//clear exec
	return (exit_code);
}
