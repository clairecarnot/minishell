#include "../../include/exec.h"

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
	// dprintf(2, "cmd = %s\n", cmd->args[0]);
	(void)ms;
	execve(cmd->args[0], cmd->args, env);
	// dprintf(2, "execve fails\n");
		// free_cmd(cmd); a verifier
		// free_tab(env); 
		// return(1) ou free minishell ?
	return (1);
}

int	exec_cmdpipe(t_ms *ms, t_ast *node, int tmp_fd)
{
	t_cmd	*cmd;
	char	**env;

	dup2(tmp_fd, STDIN_FILENO); //a proteger
	close_if(&tmp_fd); //a proteg ?

	env = lst_to_tab(ms->env);
	if (!env)
		return (1);
	cmd = node_to_cmd(node, env);
	if (!cmd)
		return (free_tab(env), 1);
	if (cmd->builtin != NOBUILT)
	{
		exec_builtin(cmd);
		// exit(0); dans chaque fonction ? ou ici ?
	}
	else
		do_cmdpipe(cmd, ms, env);
	return (1); // a verif
}

int pipex(t_ms *ms, t_ast *node, int tmp_fd, int *fd)
{
	// dprintf(2, "pipex\n");
	pid_t	pid;
	t_list	*new_pid;

	// new_pid = NULL;
	// if (!node)
		// return (0);
	if (node->type == PIPE)
	{
		// dprintf(2, "PIPE\n");
		pipex(ms, node->left, tmp_fd, fd);
		pipex(ms, node->right, tmp_fd, fd);
		// if (!node->parent || node->parent->type != PIPE)
 		//    tmp_fd = dup(STDIN_FILENO);
	}
	else if (node->type == CMD && node->parent->right == node && node->parent && node->parent->type == PIPE \
	&& (!node->parent->parent || node->parent->parent->type != PIPE))// end cmd
	{
		// dprintf(2, "END CMD\n");
		pid = fork();
		if (pid == -1)
		{
			free_minishell(ms, 1); //A VERIF
			//return code erreur
		}
		else if (pid == 0)
		{
			if (exec_cmdpipe(ms, node, tmp_fd))
				return (1); //A VERIF
		}
		else
		{
			// dprintf(2, "pid = %d\n", pid);
			close_if(&tmp_fd);
			tmp_fd = dup(STDIN_FILENO);
			new_pid = ft_lstnew_int(pid);
			if (!new_pid)
				return (1); //A PROTEGER
			ft_lstadd_back(&ms->pidlst, new_pid);
			// dprintf(2, "END CMD ---> end parent\n");
		}
	}
	else if (node->type == CMD)// middle cmd
	{
		// dprintf(2, "MID CMD\n");
		pipe(fd);// a proteger
		// dprintf(2, "pipe[0] = %d\n", fd[0]);
		// dprintf(2, "pipe[1] = %d\n", fd[1]);
		pid = fork();
		if (pid == -1)
		{
			free_minishell(ms, 1); //A VERIF
			//return code erreur
		}
		else if (pid == 0)
		{
			dup2(fd[1], STDOUT_FILENO);
			//a proteger
			close_if(&fd[0]); //a proteg ?
			close_if(&fd[1]); // a proteg ?
			if (exec_cmdpipe(ms, node, tmp_fd))
				return (1); //A VERIF
		}
		else
		{
			// dprintf(2, "pid = %d\n", pid);
			close_if(&fd[1]);
			close_if(&tmp_fd);
			// tmp_fd = fd[0];
			// close_if(&fd[0]);//
			tmp_fd = dup(fd[0]); //
			close_if(&fd[0]); //
			new_pid = ft_lstnew_int(pid);
			if (!new_pid)
				return (1); //A PROTEGER
			ft_lstadd_back(&ms->pidlst, new_pid);
		}
	}

	return (0);
}

int	exec_pipeline(t_ast *node, t_ms *ms)
{
    int tmp_fd;
    int fd[2];
	t_list	*tmp;

	fd[0] = -1;
	fd[1] = -1;
    tmp_fd = dup(STDIN_FILENO); //a proteger
	// dprintf(2, "tmp = %d\n", tmp_fd);
    pipex(ms, node, tmp_fd, fd);
	close_if(&tmp_fd);
	tmp = ms->pidlst;
	while (tmp)
	{
		waitpid(tmp->n, NULL, 0);
		tmp = tmp->next;
	}
	return (0);
}