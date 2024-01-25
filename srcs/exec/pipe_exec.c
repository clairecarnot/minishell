#include "../../include/exec.h"
#include "../../include/signals.h"

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

int	do_cmdpipe(t_cmd *cmd, t_ms *ms, char **env)
{
	close_if(&ms->in);
	close_if(&ms->out);
	if (!cmd->valid_path || cmd->is_dir)
		msg_do_cmd(ms, cmd);
	execve(cmd->args[0], cmd->args, env);
	(perror("execve failed"), free_cmd(cmd), free_minishell(ms, errno));
	return (0);
}

/*
 * exec_cmdpipe:
 * Being in this function means that we are in a child
 * Last line: free_exit and then exit, not free_minishell because
 *  if exit_code = 0, the child will not be quit
 */

int	exec_cmdpipe(t_ms *ms, t_ast *node, int tmp_fd)
{
	t_cmd	*cmd;
	char	**env;
	int		exit_code;

	exit_code = 0;
	if (dup2(tmp_fd, STDIN_FILENO) == -1)
		(perror("dup2 failed"), free_minishell(ms, errno));
	close_if(&tmp_fd);
	env = lst_to_tab(ms->env);
	if (!env)
		free_minishell(ms, 255);
	cmd = init_cmd(ms, env);
	exit_code = node_to_cmd(ms, node, cmd);
	if (ms->exit_code == 255)
		(free_cmd(cmd), free_minishell(ms, 255));
	if (exit_code != 0)
		(free_cmd(cmd), free_minishell(ms, exit_code));
	if (cmd->redir && !cmd->valid_redir)
		(free_cmd(cmd), free_exit(ms), exit(ms->exit_code));
	if (cmd->builtin != NOBUILT)
		exit_code = exec_builtin(ms, cmd);
	else
		exit_code = do_cmdpipe(cmd, ms, env);
	(free_cmd(cmd), free_exit(ms), exit(exit_code));
	return (0);
}

int	pipe_end_cmd(t_ms *ms, t_ast *node, int tmp_fd)
{
	pid_t	pid;
	t_list	*new_pid;

	pid = fork();
	if (pid == -1)
		(perror("fork failed"), free_minishell(ms, errno));
	else if (pid == 0)
	{
		child_signals();
		if (exec_cmdpipe(ms, node, tmp_fd))
			return (1); //A VERIF
	}
	else
	{
		ms_signals();
		close_if(&tmp_fd);
		tmp_fd = dup(STDIN_FILENO);
		new_pid = ft_lstnew_int(pid);
		if (!new_pid)
			return (1); //A PROTEGER
		ft_lstadd_back(&ms->pidlst, new_pid);
	}
	return (0);
}

int	pipe_middle_cmd(t_ms *ms, t_ast *node, int tmp_fd, int *fd)
{
	pid_t	pid;
	t_list	*new_pid;

	if (pipe(fd) == -1)
		(perror("pipe failed"), free_minishell(ms, errno));
	pid = fork();
	if (pid == -1)
		(perror("fork failed"), free_minishell(ms, errno));
	if (pid == -1)
		free_minishell(ms, 1); //A VERIF
	else if (pid == 0)
	{
		child_signals();
		dup2(fd[1], STDOUT_FILENO); //a proteger
		close_if(&fd[0]);
		close_if(&fd[1]);
		if (exec_cmdpipe(ms, node, tmp_fd))
			return (1); //A VERIF
	}
	else
	{
		ms_signals();
		close_if(&fd[1]);
		close_if(&tmp_fd);
		tmp_fd = dup(fd[0]); //
		close_if(&fd[0]); //
		new_pid = ft_lstnew_int(pid);
		if (!new_pid)
			return (1); //A PROTEGER
		ft_lstadd_back(&ms->pidlst, new_pid);
	}
}

int	pipex(t_ms *ms, t_ast *node, int tmp_fd, int *fd)
{
	if (node->type == PIPE)
	{
		pipex(ms, node->left, tmp_fd, fd);
		pipex(ms, node->right, tmp_fd, fd);
	}
	else if (node->type == CMD && node->parent->right == node && node->parent
		&& node->parent->type == PIPE && (!node->parent->parent
			|| node->parent->parent->type != PIPE)) // end cmd
	{
		return (pipe_end_cmd(ms, node, tmp_fd));
	}
	else if (node->type == CMD)// middle cmd
	{
		return (pipe_middle(ms, node, tmp_fd, fd));
	}
	return (0);
}

int	exec_pipeline(t_ast *node, t_ms *ms)
{
    int tmp_fd;
    int fd[2];

	fd[0] = -1;
	fd[1] = -1;
	tmp_fd = dup(STDIN_FILENO); //a proteger
	pipex(ms, node, tmp_fd, fd);
	close_if(&tmp_fd);
	return (0);
}