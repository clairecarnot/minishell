/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:18:01 by mapoirie          #+#    #+#             */
/*   Updated: 2024/02/01 17:18:04 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"
#include "../libft/libft.h"

int	cd_alone(t_ms *ms, t_cmd *cmd)
{
	char	*home_var;

	home_var = getvar_env(ms, cmd, "HOME");
	if (ms->home && home_var)
	{
		free(home_var);
		if (chdir(ms->home))
		{
			perror("chdir");
			free_cmd(cmd);
			ms->exit_code = errno;
			free_minishell(ms, errno);
		}
		replace_pwd_env_exp(ms, cmd);
		return (0);
	}
	else if (!home_var)
		return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	return (0);
}

int	cd_dash(t_ms *ms, t_cmd *cmd)
{
	t_list	*env_tmp;

	env_tmp = ms->env;
	if (cmd->args[1][1])
		return ((ft_putstr_fd("minishell: cd: ", 2), ft_putstr_fd \
		(cmd->args[1], 2), ft_putstr_fd(": invalid option\n", 2)), 2);
	else
	{
		while (env_tmp)
		{
			if (ft_strncmp(env_tmp->content, "OLDPWD", 6) == 0)
			{
				printf("%s\n", (char *)env_tmp->content + 7);
				if (chdir(env_tmp->content + 7))
				{
					(perror("chdir"), ms->exit_code = errno);
					(free_cmd(cmd), free_minishell(ms, errno));
				}
				replace_pwd_env_exp(ms, cmd);
				return (0);
			}
			env_tmp = env_tmp->next;
		}
		return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), 1);
	}
}

int	cd_tilde(t_ms *ms, t_cmd *cmd)
{
	if (ms->home)
	{
		if (chdir(ms->home))
		{
			perror("chdir");
			free_cmd(cmd);
			ms->exit_code = errno;
			free_minishell(ms, errno);
		}
	}
	replace_pwd_env_exp(ms, cmd);
	return (0);
}

int	cd_slash(t_ms *ms, t_cmd *cmd)
{
	if (chdir(cmd->args[1]) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	else
		return (replace_pwd_env_exp(ms, cmd), 0);
}

int	exec_cd(t_ms *ms, t_cmd *cmd)
{
	if (cmd->args && cmd->args[1] && cmd->args[2])
	{
		ms->exit_code = 1;
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	}
	if (cmd->args && !cmd->args[1])
		return (cd_alone(ms, cmd));
	if (cmd->args && cmd->args[1])
	{
		if (cmd->args[1][0] == '-')
			return (cd_dash(ms, cmd));
		else if (cmd->args[1][0] == '~' && !cmd->args[1][1])
			return (cd_tilde(ms, cmd));
		else if (cmd->args[1][0] == '/')
			return (cd_slash(ms, cmd));
		else if (cmd->args[1][0] != '/')
			return (cd_else(ms, cmd));
	}
	return (0);
}
