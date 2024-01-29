#include "../../include/exec.h"

int	pipe_end_cmd(t_ms *ms, t_ast *node, int tmp_fd)
{
	pid_t	pid;
	t_list	*new_pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork fail"), kill_loop(ms), errno);
	else if (pid == 0)
	{
		child_signals();
		return (exec_cmdpipe(ms, node, tmp_fd));
	}
	else
	{
		(ms_signals(), close_if(&tmp_fd));
		tmp_fd = dup(STDIN_FILENO);
		if (tmp_fd == -1)
			return (perror("dup failed"), kill_loop(ms),
				kill(pid, SIGKILL), errno);
		new_pid = ft_lstnew_int(pid);
		if (!new_pid)
			return (kill_loop(ms), kill(pid, SIGKILL), ms->exit_code = 255);
		ft_lstadd_back(&ms->pidlst, new_pid);
	}
	return (0);
}

int	parent_middle(t_ms *ms, int tmp_fd, int *fd, pid_t pid)
{
	t_list	*new_pid;

	ms_signals();
	close_if(&fd[1]);
	close_if(&tmp_fd);
	tmp_fd = dup(fd[0]);
	if (tmp_fd == -1)
	{
		(perror("dup failed"), close_if(&fd[0]), kill_loop(ms));
		kill(pid, SIGKILL);
		return (ms->exit_code = errno);
	}
	close_if(&fd[0]);
	new_pid = ft_lstnew_int(pid);
	if (!new_pid)
	{
		(kill_loop(ms), kill(pid, SIGKILL));
		return (ms->exit_code = 255);
	}
	ft_lstadd_back(&ms->pidlst, new_pid);
	return (0);
}

int	pipe_middle_cmd(t_ms *ms, t_ast *node, int tmp_fd, int *fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork fail"), close_if(&fd[1]), close_if(&fd[0]), errno);
	else if (pid == 0)
	{
		child_signals();
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			(perror("dup2 failed"), close_if(&fd[0]), close_if(&fd[1]),
				close_if(&tmp_fd), free_minishell(ms, errno));
		(close_if(&fd[0]), close_if(&fd[1]));
		return (exec_cmdpipe(ms, node, tmp_fd));
	}
	else
	{
		return (parent_middle(ms, tmp_fd, fd, pid));
	}
}

int	pipex(t_ms *ms, t_ast *node, int tmp_fd, int *fd)
{
	int	exit_code;

	exit_code = 0;
	if (node->type == PIPE)
	{
		exit_code = pipex(ms, node->left, tmp_fd, fd);
		if (exit_code != 0)
			return (exit_code);
		exit_code = pipex(ms, node->right, tmp_fd, fd);
		if (exit_code != 0)
			return (exit_code);
	}
	else if (node->type == CMD && node->parent->right == node && node->parent
		&& node->parent->type == PIPE && (!node->parent->parent
			|| node->parent->parent->type != PIPE))
	{
		return (pipe_end_cmd(ms, node, tmp_fd));
	}
	else if (node->type == CMD)
	{
		if (pipe(fd) == -1)
			return (perror("pipe failed"), ms->exit_code = errno);
		return (pipe_middle_cmd(ms, node, tmp_fd, fd));
	}
	return (0);
}

int	exec_pipeline(t_ast *node, t_ms *ms)
{
	int	tmp_fd;
	int	fd[2];
	int	exit_code;

	exit_code = 0;
	fd[0] = -1;
	fd[1] = -1;
	tmp_fd = dup(STDIN_FILENO);
	if (tmp_fd == -1)
		return (perror("dup failed"), ms->exit_code = errno);
	exit_code = pipex(ms, node, tmp_fd, fd);
	close_if(&tmp_fd);
	close_if(&fd[0]);
	close_if(&fd[1]);
	return (ms->exit_code = exit_code);
}
