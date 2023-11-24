#include "../../include/exec.h"

int	equals(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (0);
	if (ft_strlen(s1) != ft_strlen(s2))
		return (0);
	if (ft_strncmp(s1, s2, ft_strlen(s1)) == 0)//options ?
		return (1);
	return (0);
}

t_builtin_type	builtin_type_is(char *cmd)
{
	if (equals(cmd, "echo")
		return (ECHO);
	if (equals(cmd, "cd")
		return (ECHO);
	if (equals(cmd, "cd")
		return (CD);
	if (equals(cmd, "pwd")
		return (PWD);
	if (equals(cmd, "export")
		return (EXPORT);
	if (equals(cmd, "unset")
		return (UNSET);
	if (equals(cmd, "env")
		return (ENV);
	if (equals(cmd, "exit")
		return (EXIT);
	return (NOBUILT);
}

char	**lst_to_tab(t_list *lst)
{
	char	**args;
	t_list	*tmp;

	tmp = lst;
	args = ft_calloc(ft_lstsize, sizeof(char*));
	if (!args)
		return (NULL);
	while (tmp)
	{
		*args = ft_strdup(tmp->content);
		if (!args)
			return (free_tab(args), NULL);
		args++;
		tmp = tmp->next;
	}
	return (args);
}

t_cmd	*node_to_cmd(t_ast *node, t_ms *ms)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = lst_to_tab(node->args);
	if (!cmd->args)
		return (free_cmd(cmd), NULL);
	cmd->builtin = builtin_type_is(cmd->args[0]);
	return (cmd);
}

int	exec_builtin(t_cmd *cmd, t_ms *ms)
{
	if (cmd->builtin == ECHO)
		return (exec_echo);
	if (cmd->builtin == CD)
		return (exec_cd);
	if (cmd->builtin == pwd)
		return (exec_pwd);
	if (cmd->builtin == export)
		return (exec_export);
	if (cmd->builtin == unset)
		return (exec_unset);
	if (cmd->builtin == env)
		return (exec_env);
	if (cmd->builtin == exit)
		return (exec_exit);

	return (1);
}

int	do_cmd(t_cmd *cmd, t_ms *ms, char **env)
{
	int	pid;
	t_list	*new_pid;

	pid = fork();
	if (pid == -1)
		return (1);
	else if (pid == 0)
	{
		execve(cmd[0], cmd, env);
		//execve fails
		clear_cmd(cmd);
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
	cmd = node_to_cmd(node, ms);
	if (!cmd)
		return (free_tab(env), 1);
	if (cmd->builtin != NOBUILT)
		g_exit_code = exec_builtin(cmd, ms);
	else
		g_exit_code = do_cmd(cmd, ms, env);
	//parent waitpid?
	free_cmd(cmd);
	free_tab(cmd);
	return (0);
}
