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

int	msg_do_cmd(t_ms *ms, t_cmd *cmd)
{
	if (!cmd->args || !cmd->args[0] || !ft_strlen(cmd->args[0]))
	{
		if (ms->flag_q == 1)
			ft_putstr_fd("minishell: : command not found\n", 2);
		(free_cmd(cmd), free_minishell(ms, 127));
	}
	if (cmd->is_dir)
	{
		(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd->args[0], 2),
			ft_putstr_fd(": Is a directory\n", 2));
		(free_cmd(cmd), free_minishell(ms, 126));
	}
	else if (cmd->abs_or_rel)
	{
		(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd->args[0], 2),
			ft_putstr_fd(": No such file or directory\n", 2));
		(free_cmd(cmd), free_minishell(ms, 127));
	}
	else
	{
		(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd->args[0], 2),
			ft_putstr_fd(": command not found\n", 2));
		(free_cmd(cmd), free_minishell(ms, 127));
	}
	return (0);
}

void	replace_var_underscore(t_ms *ms, t_cmd *cmd)
{
	char	*new_content;
	t_list	*tmp;

	new_content = NULL;
	tmp = ms->env;
	while (tmp)
	{
		if (ft_strncmp("_=/usr/bin", tmp->content, 10) == 0)
		{
			if (cmd->args[0] && cmd->args[0][0] == '/')
				new_content = ft_strjoin("_=", cmd->args[0]);
			else if (cmd->args[0] && cmd->args[0][0] != '/')
				new_content = ft_strjoin("_=/usr/bin/", cmd->args[0]);
			else if (!cmd->args || !cmd->args[0])
				new_content = ft_strdup("_=/usr/bin/");
			if (!new_content)
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
