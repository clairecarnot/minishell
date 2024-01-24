#include "../../include/exec.h"
#include "../../include/signals.h"

t_cmd	*init_cmd(t_ms *ms, char **env)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
	{
		free_tab(env);
		free_minishell(ms, 255);
	}
	cmd->env = env;
	cmd->bin_paths = get_bin_paths(ms, env, cmd);// c'est verifie
	cmd->abs_or_rel = 0;
	cmd->valid_path = 0;
	cmd->redir = 0;
	cmd->valid_redir = 0;
	cmd->is_dir = 0;
	cmd->invalid_io = NULL;
	cmd->args = NULL;
	return (cmd);
}

int	redef_cmdargs_bis(t_ms *ms, t_cmd *cmd, int i)
{
	char	**tmp;

	tmp = tab_cpy(ms, &cmd->args[i]);
	if (!tmp)
	{
		ms->exit_code = 255;
		return (255);
	}
	while (cmd->args[i])
	{
		free(cmd->args[i]);
		cmd->args[i] = NULL;
		i++;
	}
	free(cmd->args);
	cmd->args = tmp;
	return (0);
}

int	redef_cmdargs(t_ms *ms, t_cmd *cmd)
{
	int		i;

	i = 0;
	while (cmd->args && cmd->args[i] && ft_strlen(cmd->args[i]) == 0
		&& !ms->flag_q)
	{
		free(cmd->args[i]);
		cmd->args[i] = NULL;
		i++;
	}
	if (!cmd->args[i] || !ft_strlen(cmd->args[i]))
	{
		if (!ms->flag_q)
			*cmd->args = NULL;
		return (0);
	}
	if (i > 0)
		return (redef_cmdargs_bis(ms, cmd, i));
	return (0);
}

int	node_to_cmd_bis(t_ms *ms, t_ast *node, t_cmd *cmd, t_list *tmp_w)
{
	if (node->wil)
	{
		if (cmd_wildcard(cmd, node->wil) == 1)
		{
			node->wil->w = tmp_w;
			free_cmd(cmd);
			prefree_minishell(ms, NULL);
			// return (255);// a verifier
		}
		node->wil->w = tmp_w;
	}
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
		return (abs_rel_path(ms, cmd));
	else
	{
		cmd->builtin = builtin_type_is(cmd->args[0]);
		if (cmd->builtin != NOBUILT)
		{
			cmd->valid_path = 1;
			return (0);
		}
		if (build_path(cmd) == 1)
			return (255);
	}
	return (0);
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
		return (255);
	if (node->dol)
	{
		if (cmd_expand(ms, cmd, node->dol) == 1) //1 = bad malloc
		{
			save_ptrs(&tmp_d, &tmp_c, &node->dol->d, &node->dol->c);
			return (255);
		}
		save_ptrs(&tmp_d, &tmp_c, &node->dol->d, &node->dol->c);
	}
	if (redef_cmdargs(ms, cmd) == 1)
		return (255);
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

	if (!cmd->valid_path || cmd->is_dir)
	{
		if (!cmd->args || !cmd->args[0] || !ft_strlen(cmd->args[0]))
		{
			if (ms->flag_q == 1)
				ft_putstr_fd("minishell: : command not found\n", 2);
			ms->flag_q--;
			return (0);
		}
		if (cmd->is_dir)
			(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd->args[0], 2),
				ft_putstr_fd(": Is a directory\n", 2));
		else if (cmd->abs_or_rel)
			(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd->args[0], 2),
				ft_putstr_fd(": No such file or directory\n", 2));
		else
			(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd->args[0], 2),
				ft_putstr_fd(": command not found\n", 2));
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
		child_signals();
		close_if(&ms->in);
		close_if(&ms->out);
		execve(cmd->args[0], cmd->args, env);
		dprintf(2, "execve fails\n");
		(free_cmd(cmd), free_minishell(ms, errno)); // on exit ms, code err?
	}
	else
	{
		ms_signals();
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

void	replace_var_underscore(t_ms *ms, t_cmd *cmd)
{
	char	*new_content;
	t_list	*tmp;

	tmp = ms->env;
	while (tmp)
	{
		if (ft_strncmp("_=/usr/bin", tmp->content, 10) == 0)
		{
			if (cmd->args[0] && cmd->args[0][0] == '/')
				new_content = ft_strjoin("_=", cmd->args[0]);
			else if (cmd->args[0] && cmd->args[0][0] != '/')
				new_content = ft_strjoin("_=/usr/bin/", cmd->args[0]);
			if (!new_content) // c'est verifie mais reouvre le programme pour afficher l'exec.. /?/
			{
				free_cmd(cmd);
				free_minishell(ms, 1);
			}
			free(tmp->content);
			tmp->content = new_content;
		}
		tmp = tmp->next;
	}
}

int	exec_cmd(t_ast *node, t_ms *ms)
{
	t_cmd	*cmd;
	char	**env;
	int		exit_code;

	exit_code = 0;
	env = lst_to_tab(ms->env);
	if (!env)
		return (ms->exit_code = 255, 1);// a verifie
	cmd = init_cmd(ms, env);// c'est verifie
	if (!cmd)
		return (ms->exit_code = 255, 1);
	exit_code = node_to_cmd(ms, node, cmd);
	if (ms->exit_code == 255)
		(free_cmd(cmd), free_minishell(ms, 255));
	if (exit_code != 0)
		return (ms->exit_code = exit_code, free_cmd(cmd), 1);
	if (cmd->redir && !cmd->valid_redir)
		return (free_cmd(cmd), 1);
	if (cmd->builtin != NOBUILT)
		exit_code = exec_builtin(ms, cmd);
	else
		exit_code = do_cmd(cmd, ms, env);
	replace_var_underscore(ms, cmd);
	return (ms->exit_code = exit_code, free_cmd(cmd), exit_code);
}
