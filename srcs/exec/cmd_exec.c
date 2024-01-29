#include "../../include/exec.h"

int	node_to_cmd_bis(t_ms *ms, t_ast *node, t_cmd *cmd, t_list *tmp_w)
{
	if (node->wil)
	{
		if (cmd_wildcard(cmd, node->wil) == 1)
		{
			node->wil->w = tmp_w;
			return (ms->exit_code = 255, 255);
		}
		node->wil->w = tmp_w;
	}
	if ((cmd->args && cmd->args[0][0]) && (cmd->args[0][0] == '/'
			|| cmd->args[0][0] == '.'))
		return (abs_rel_path(ms, cmd));
	else
	{
		cmd->builtin = builtin_type_is(cmd->args[0]);
		if (cmd->builtin != NOBUILT)
		{
			cmd->valid_path = 1;
			return (0);
		}
		return (build_path(ms, cmd));
	}
}

int	node_to_cmd(t_ms *ms, t_ast *node, t_cmd *cmd)
{
	t_list	*tmp_d;
	t_list	*tmp_c;
	t_list	*tmp_w;

	if (node->redirs && cmd_redirs(ms, node, cmd))
		return (1);
	if (node->dol)
		save_ptrs(&node->dol->d, &node->dol->c, &tmp_d, &tmp_c);
	if (node->wil)
		tmp_w = node->wil->w;
	cmd->args = lst_to_tab(node->args);
	if (!cmd->args)
		return (ms->exit_code = 255, 255);
	if (node->dol)
	{
		if (cmd_expand(ms, cmd, node->dol) == 1)
		{
			save_ptrs(&tmp_d, &tmp_c, &node->dol->d, &node->dol->c);
			return (255);
		}
		save_ptrs(&tmp_d, &tmp_c, &node->dol->d, &node->dol->c);
	}
	if (redef_cmdargs(ms, cmd) != 0)
		return (1);
	return (node_to_cmd_bis(ms, node, cmd, tmp_w));
}

int	exec_builtin(t_ms *ms, t_cmd *cmd)
{
	if (cmd->builtin == ECHO)
		return (exec_echo(ms, cmd));
	if (cmd->builtin == CD)
		return (exec_cd(ms, cmd));
	if (cmd->builtin == PWD)
		return (exec_pwd(ms, cmd));
	if (cmd->builtin == EXPORT)
		return (exec_export(ms, cmd));
	if (cmd->builtin == UNSET)
		return (exec_unset(ms, cmd));
	if (cmd->builtin == ENV)
		return (exec_env(ms, cmd));
	if (cmd->builtin == EXIT)
		return (exec_exit(ms, cmd));
	return (1);
}

int	do_cmd(t_cmd *cmd, t_ms *ms, char **env)
{
	int		pid;
	t_list	*new_pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork failed"), ms->exit_code = errno, errno);
	else if (pid == 0)
	{
		(child_signals(), close_if(&ms->in), close_if(&ms->out));
		if (!cmd->valid_path || cmd->is_dir)
			msg_do_cmd(ms, cmd);
		execve(cmd->args[0], cmd->args, env);
		(perror("execve failed"), free_cmd(cmd), free_minishell(ms, errno));
	}
	else
	{
		ms_signals();
		ms->flag_q--;
		new_pid = ft_lstnew_int(pid);
		if (!new_pid)
			return (kill_loop(ms), kill(pid, SIGKILL),
				ms->exit_code = 255, 255);
		ft_lstadd_back(&ms->pidlst, new_pid);
	}
	return (0);
}

int	exec_cmd(t_ast *node, t_ms *ms)
{
	t_cmd	*cmd;
	char	**env;
	int		exit_code;

	exit_code = 0;
	env = lst_to_tab(ms->env);
	if (!env)
		free_minishell(ms, 255);
	cmd = init_cmd(ms, env);
	exit_code = node_to_cmd(ms, node, cmd);
	if (ms->exit_code == 255)
		(free_cmd(cmd), free_minishell(ms, 255));
	if (exit_code != 0)
		return (ms->exit_code = exit_code, free_cmd(cmd), exit_code);
	if (cmd->redir && !cmd->valid_redir)
		return (free_cmd(cmd), ms->exit_code);
	if (cmd->builtin != NOBUILT)
		exit_code = exec_builtin(ms, cmd);
	else
		exit_code = do_cmd(cmd, ms, env);
	if (ms->exit_code == 255)
		(free_cmd(cmd), free_minishell(ms, 255));
	replace_var_underscore(ms, cmd);
	return (ms->exit_code = exit_code, free_cmd(cmd), exit_code);
}
