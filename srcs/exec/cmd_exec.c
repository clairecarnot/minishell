#include "../../include/exec.h"
#include "../../include/signals.h"

t_cmd	*node_to_cmd(t_ms *ms, t_ast *node, char **env)
{
	t_cmd	*cmd;
	t_list	*tmp_d;
	t_list	*tmp_c;
	char	**tmp;

	if (node->dol)
	{
		if (!node->dol->d)
			dprintf(2, "dol->d n'existe pas\n");
//		dprintf(2, "dol = %d\n", node->dol->d->n);
		tmp_d = node->dol->d;
		tmp_c = node->dol->c;
	}
	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->env = NULL;
	cmd->bin_paths = NULL;
	cmd->abs_or_rel = 0;
	cmd->valid_path = 0;
	cmd->redir = 0;
	cmd->valid_redir = 0;
	cmd->invalid_io = NULL;
//	cmd->args = NULL;
	//	cmd->tmp_args = lst_to_tab(node->args);
	cmd->args = lst_to_tab(node->args);
	//	if (!cmd->tmp_args)
	//		return (free_cmd(cmd), NULL);
	//	cmd->args = tab_cpy(ms, cmd->tmp_args);
	if (!cmd->args)
		return (free_cmd(cmd), NULL);
	if (node->dol)
	{
		if (cmd_expand(ms, cmd->args, node->dol) == 1)
		{
			node->dol->d = tmp_d;
			node->dol->c = tmp_c;
			return (free_cmd(cmd), NULL); // A CHECKER
		}
		node->dol->d = tmp_d;
		node->dol->c = tmp_c;
		//		cmd->args = post_expand_adj(ms, cmd->args, cmd->tmp_args);
		//		if (!cmd->args)
		//			return (free_cmd(cmd), NULL); // A CHECKER
	}
	int	i;
	i = 0;
	while (cmd->args && cmd->args[i] && ft_strlen(cmd->args[i]) == 0 && !ms->flag_q)
	{
//		dprintf(2, "cur args[i] = %s\n", cmd->args[i]);
		free(cmd->args[i]);
		cmd->args[i] = NULL;
		i++;
//		if (!cmd->args[i])
//		{
//			dprintf(2, "sortie boucle\n");
//			cmd->args = NULL;
//			break ;
//		}
//		cmd->args = &cmd->args[i];
//	if (!cmd->args || !cmd->args[0])
	}
	if (!cmd->args[i] || !ft_strlen(cmd->args[i]))
	{
		if (!ms->flag_q)
			*cmd->args = NULL;
//		free(cmd->args);
//		cmd->args = NULL;
//		if (cmd->args && !cmd->args[0])
//			cmd->args = NULL;
		return (cmd);
	}
	if (i > 0)
	{
		tmp = tab_cpy(ms, &cmd->args[i]);
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			cmd->args[i] = NULL;
			i++;
		}
		free(cmd->args);
		cmd->args = tmp;
	}
//	else
//		cmd->args = &cmd->args[i];
	//	dprintf(2, "arg[0] exists\n");
	//	dprintf(2, "%s\n", cmd->args[0]);
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
	if (node->redirs)
		cmd_redirs(ms, node, cmd);
//		if (cmd_redirs(ms, node, cmd) == 1)
//			return (free_cmd(cmd), NULL); // A CHECKER
	// dprintf(2, "return cmd\n");
	return (cmd);
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

	if (!cmd->valid_path)
	{
//		dprintf(2, "no valid path\n");
		if (!cmd->args || !cmd->args[0] || !ft_strlen(cmd->args[0]))
		{
			if (ms->flag_q == 1)
				ft_putstr_fd("minishell: : command not found\n", 2);
			ms->flag_q--;
			return (0);
		}
		if (cmd->abs_or_rel) //printf mais sur sortie d'erreur ?
			(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd->args[0], 2), ft_putstr_fd(": No such file or directory\n", 2));
		else
			(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd->args[0], 2), ft_putstr_fd(": command not found\n", 2));
		ms->exit_code = 127;
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		ms->exit_code = errno; //A CHECKER
		return (1);
	}
	else if (pid == 0)
	{
//		int j = 0;
//		while (cmd->args[j])
//		{
//			dprintf(2, "%s\n", cmd->args[j]);
//			j++;
//		}
		//		dprintf(2, "%s\n", cmd->args[0]);
		child_signals();
		close_if(&ms->in);
		close_if(&ms->out);
		execve(cmd->args[0], cmd->args, env);
		dprintf(2, "execve fails\n");
		(free_cmd(cmd), free_minishell(ms, errno)); // on exit ms, code err?
	}
	else
	{
//		dprintf(2, "pid2 = %d\n", pid);
		//signal(SIGINT, SIG_IGN);
//		signal(SIGINT, SIG_IGN);
		ms_signals();
		// preprompt_signals();
		//postprompt_signals();
		ms->flag_q--;
		new_pid = ft_lstnew_int(pid);
		if (!new_pid)
		{
			ms->exit_code = 255;
			return (1);
		}
		ft_lstadd_back(&ms->pidlst, new_pid);
	}
	return (0);
}

int	exec_cmd(t_ast *node, t_ms *ms)
{
	t_cmd	*cmd;
	char	**env;
	t_list	*tmp;
	int	exit_code;
	int	status;
	int	print;

	status = 0;
	print = 1;
	exit_code = 0;
	env = lst_to_tab(ms->env);
	if (!env)
		return (1);
	cmd = node_to_cmd(ms, node, env);
	if (!cmd)
		return (free_tab(env), 1);
	cmd->env = env;
	if (cmd->redir && !cmd->valid_redir)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->invalid_io, 2);
//		ft_putstr_fd(": No such file or directory\n", 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return (free_cmd(cmd), 1);
	}
	if (cmd->builtin != NOBUILT)
		exit_code = exec_builtin(ms, cmd);
	else
		exit_code = do_cmd(cmd, ms, env);
	tmp = ms->pidlst;
//	waitpid(tmp->n, &status, WUNTRACED);
//	dprintf(2, "pid == %d\n", tmp->n);
//	waitpid(tmp->n, &status, WUNTRACED);
//	dprintf(2, "pid == %d\n", tmp->n);
//	while (wait(&status) > 0)
	/*
	while (waitpid(tmp->n, &status, WUNTRACED) > 0)
	{
	if (WIFEXITED(status))
	{
		dprintf(2, "exit code recuuuuuuu\n");
		dprintf(2, "code = %d\n", WEXITSTATUS(status));
		ms->exit_code = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		dprintf(2, "signal recu\n");
		ms->exit_code = 128 + WTERMSIG(status);
		if (print)
		{
			if (WTERMSIG(status) == SIGQUIT)
				ft_putstr_fd("Quit (core dumped)\n", 2);
			else if (WTERMSIG(status) == SIGINT)
			{
				dprintf(2, "signal = sigint\n");
				ft_putstr_fd("\n", 2);
			}
			print = 0;
		}
	}
	}
	*/
	// tmp = ms->pidlst;
	// while (tmp)
	// {
	// 	waitpid(tmp->n, NULL, 0);
	// 	tmp = tmp->next;
	// }
//	free_tab(env);
	return (free_cmd(cmd), exit_code);
}
