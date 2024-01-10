/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:13:01 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/10 18:40:22 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"
#include "../../include/signals.h"

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

int	execute(t_ast *node, t_ms *ms) //return exit_code?
{
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
	int	status;
	int	print;	
	// t_list	*hdtmp;
	// pid_t	pid;

	status = 0;
	print = 1;
	ms->in = dup(STDIN_FILENO); //A PROTEGER?
	ms->out = dup(STDOUT_FILENO);//A PROTEGER?
	//handle save STDIN STDOUT
	//handle signals
	exit_code = execute(ms->root, ms);
	if (exit_code)
		return (1);
	// dprintf(2, "after exec, before waitpid\n");
	tmp = ms->pidlst;
	while (tmp)
	{
		dprintf(2, "pid = %d\n", tmp->n);
		waitpid(tmp->n, &status, 0);
		tmp = tmp->next;
	}
	/*
	int ret  = waitpid(tmp->n, &status, WUNTRACED);
	dprintf(2, "%d\n", ret);
	perror("waitpid 1");
	ret = waitpid(tmp->n, &status, WUNTRACED);
	dprintf(2, "%d\n", ret);
	perror("waitpid 2");
	*/
	if (WIFEXITED(status))
	{
		dprintf(2, "exit code recu\n");
		dprintf(2, "code = %d\n", WEXITSTATUS(status));
		ms->exit_code = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		dprintf(2, "signal recu2\n");
		dprintf(2, "signal = %d\n", WTERMSIG(status));
		ms->exit_code = 128 + WTERMSIG(status);
		if (print)
		{
			if (WTERMSIG(status) == SIGQUIT)
				ft_putstr_fd("Quit (core dumped)\n", 2);
			else if (WTERMSIG(status) == SIGINT)
			{
				;
//				dprintf(2, "signal = sigint\n");
//				ft_putstr_fd("\n", 2);
			}
			print = 0;
		}
	}
	dup2(ms->in, STDIN_FILENO);
	dup2(ms->out, STDOUT_FILENO);
	//postprompt_signals();
	//preprompt_signals();
	// close_if(&ms->in);
	// close_if(&ms->out);
	// close_if("/tmp/here_doc");
	// hdtmp = ms->hdlst;
	// while (hdtmp)
	// {
	// 	if (hdtmp->content)
	// 		unlink(hdtmp->content);
	// 	hdtmp = hdtmp->next;
	// }
	// dprintf(2, "after exec, after waitpid\n");
	//clear exec
	return (exit_code);
}
