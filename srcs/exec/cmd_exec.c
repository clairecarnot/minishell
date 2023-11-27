#include "../../include/exec.h"

t_cmd	*node_to_cmd(t_ast *node, char **env)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->bin_paths = NULL;
	cmd->abs_or_rel = 0;
	cmd->valid_path = 0;
	cmd->args = lst_to_tab(node->args);
	if (!cmd->args)
		return (free_cmd(cmd), NULL);
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
		abs_rel_path(cmd);
	else
	{
		cmd->builtin = builtin_type_is(cmd->args[0]);
		if (cmd->builtin != NOBUILT)
		{
			cmd->valid_path = 1;
			return (cmd);
		}
		cmd->bin_paths = get_bin_paths(env);
		build_path(cmd);
	}
	return (cmd);
}

int	exec_builtin(t_cmd *cmd)
{
	if (cmd->builtin == ECHO)
		return (0); //A remplacer par ligne ci-dessous
//		return (exec_echo);
	if (cmd->builtin == CD)
		return (0); //A remplacer par ligne ci-dessous
//		return (exec_cd);
	if (cmd->builtin == PWD)
		return (0); //A remplacer par ligne ci-dessous
//		return (exec_pwd);
	if (cmd->builtin == EXPORT)
		return (0); //A remplacer par ligne ci-dessous
//		return (exec_export);
	if (cmd->builtin == UNSET)
		return (0); //A remplacer par ligne ci-dessous
//		return (exec_unset);
	if (cmd->builtin == ENV)
		return (0); //A remplacer par ligne ci-dessous
//		return (exec_env);
	if (cmd->builtin == EXIT)
		return (0); //A remplacer par ligne ci-dessous
//		return (exec_exit);
	return (1);
}

int	do_cmd(t_cmd *cmd, t_ms *ms, char **env)
{
	int		pid;
	t_list	*new_pid;

	pid = fork();
	if (pid == -1)
		return (1);
	else if (pid == 0)
	{
		execve(cmd->args[0], cmd->args, env);
		dprintf(2, "execve fails\n");
		free_cmd(cmd);
		exit(1);
	}
	else
	{
		new_pid = ft_lstnew(&pid);
		if (!new_pid)
			return (1); //A PROTEGER
		ft_lstadd_back(&ms->pidlst, new_pid);
	}
	//parent waitpid?
	return (0);
}

int	exec_cmd(t_ast *node, t_ms *ms)
{
	t_cmd	*cmd;
	char	**env;

	env = lst_to_tab(ms->env);
	if (!env)
		return (1);
	cmd = node_to_cmd(node, env);
	if (!cmd)
		return (free_tab(env), 1);
	if (cmd->builtin != NOBUILT)
		exec_builtin(cmd);
	else
		do_cmd(cmd, ms, env);
	//parent waitpid?
	free_cmd(cmd);
	free_tab(env);
	return (0);
}
