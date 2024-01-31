/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:12:48 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/31 14:21:52 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	abs_rel_path(t_ms *ms, t_cmd *cmd)
{
	char	*dir;

	cmd->abs_or_rel = 1;
	if (access(cmd->args[0], F_OK | X_OK) == 0)
		cmd->valid_path = 1;
	cmd->builtin = NOBUILT;
	dir = NULL;
	dir = ft_slash_addback(cmd->args[0]);
	if (!dir)
	{
		ms->exit_code = 255;
		return (255);
	}
	if (access(dir, F_OK | X_OK) == 0)
		cmd->is_dir = 1;
	free(dir);
	return (0);
}

char	*get_bin_path_underscore(t_ms *ms, t_cmd *cmd, char *env_path)
{
	char	*env_p;

	env_p = ft_strdup("/usr/bin/");
	if (!env_p)
		free_path_cmd_ms(ms, cmd, env_path);
	return (env_p);
}

void	free_path_cmd_ms(t_ms *ms, t_cmd *cmd, char *env_path)
{
	free_if(env_path);
	free_cmd(cmd);
	free_minishell(ms, 255);
}

char	**get_bin_paths(t_ms *ms, char **env, t_cmd *cmd)
{
	int		i;
	char	*env_path;
	char	**binaries;

	i = 0;
	env_path = NULL;
	while (env && env[i])
	{
		if (ft_strncmp("PATH", env[i], 4) == 0)
		{
			env_path = ft_strdup(env[i] + 5);
			if (!env_path)
				free_path_cmd_ms(ms, cmd, env_path);
			break ;
		}
		i++;
	}
	if (!env_path)
		env_path = get_bin_path_underscore(ms, cmd, env_path);
	binaries = ft_split(env_path, ':');
	if (!binaries)
		free_path_cmd_ms(ms, cmd, env_path);
	free(env_path);
	return (binaries);
}

int	build_path(t_ms *ms, t_cmd *cmd)
{
	char	*path;
	int		i;

	i = 0;
	while (cmd->bin_paths && cmd->bin_paths[i])
	{
		path = ft_strjoin_slash(cmd->bin_paths[i], cmd->args[0]);
		if (!path)
			(free_cmd(cmd), free_minishell(ms, 255));
		if (access(path, F_OK | X_OK) == 0)
		{
			(free(cmd->args[0]));
			cmd->args[0] = path;
			cmd->valid_path = 1;
			return (0);
		}
		free(path);
		i++;
	}
	return (0);
}
