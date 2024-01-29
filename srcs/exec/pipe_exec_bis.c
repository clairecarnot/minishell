#include "../../include/exec.h"

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

int	exec_cmdpipe_bis(t_ms *ms, char **env, t_cmd *cmd)
{
	t_trash	*trash;
	int		exit_code;

	exit_code = 0;
	trash = ft_calloc(1, sizeof(t_trash));
	if (!trash)
		(free_cmd(cmd), free_minishell(ms, 255));
	trash->ms = ms;
	trash->cmd = cmd;
	go_garbage(0, trash);
	child_signals_bis();
	if (cmd->builtin != NOBUILT)
		exit_code = exec_builtin(ms, cmd);
	else
		exit_code = do_cmdpipe(cmd, ms, env);
	(free_cmd(cmd), free_exit(ms), exit(exit_code));
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
	return (exec_cmdpipe_bis(ms, env, cmd));
}

void	kill_loop(t_ms *ms)
{
	t_list	*tmp;

	tmp = ms->pidlst;
	while (tmp)
	{
		kill(tmp->n, SIGKILL);
		tmp = tmp->next;
	}
}
