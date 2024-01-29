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
	int	exit_code;

	exit_code = 0;
	if (!node)
		return (0);
	if (node->type == CMD)
		exit_code = exec_cmd(node, ms);
	if (node->type == AND_IF)
		exit_code = exec_andif(node, ms);
	if (node->type == OR_IF)
		exit_code = exec_orif(node, ms);
	if (node->type == PIPE)
		exit_code = exec_pipeline(node, ms);
	return (exit_code);
}

void	wait_loop(t_ms *ms)
{
	t_list	*tmp;
	int		status;
	int		print;	

	status = 0;
	print = 1;
	tmp = ms->pidlst;
	while (tmp)
	{
		waitpid(tmp->n, &status, 0);
		tmp = tmp->next;
	}
	if (WIFEXITED(status))
		ms->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		ms->exit_code = 128 + WTERMSIG(status);
		if (print)
		{
			if (WTERMSIG(status) == SIGQUIT)
				ft_putstr_fd("Quit (core dumped)\n", 2);
			print = 0;
		}
	}
}

int	pre_exec(t_ms *ms)
{
	int		exit_code;

	ms->in = dup(STDIN_FILENO);
	if (ms->in == -1)
		return (perror("dup failed"), ms->exit_code = errno);
	ms->out = dup(STDOUT_FILENO);
	if (ms->out == -1)
		return (perror("dup failed"), ms->exit_code = errno);
	if (open_heredocs(ms, ms->root) == 1)
		return (ms->exit_code);
	exit_code = execute(ms->root, ms);
	if (dup2(ms->in, STDIN_FILENO) == -1)
		return (kill_loop(ms), perror("dup2 failed"), ms->exit_code = errno);
	if (dup2(ms->out, STDOUT_FILENO) == -1)
		return (kill_loop(ms), perror("dup2 failed"), ms->exit_code = errno);
	if (exit_code)
		return (kill_loop(ms), exit_code);
	wait_loop(ms);
	return (ms->exit_code);
}
