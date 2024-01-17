/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:12:48 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/17 14:38:42 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

void	abs_rel_path(t_cmd *cmd)
{
	cmd->abs_or_rel = 1;
	if (access(cmd->args[0], F_OK | X_OK) == 0)
		cmd->valid_path = 1;
	//dprintf(2, "ok\n");
	cmd->builtin = NOBUILT;
}

char	*get_bin_path_underscore(char **env)
{
	int		i;
	char	*env_path;

	i = 0;
	while(env && env[i])
	{
		if (ft_strncmp("_", env[i], 1) == 0)
		{
			env_path = ft_strdup("/usr/bin/");//a proteger
			return (env_path);
		}
		i++;
	}
	return (NULL);
}

char	**get_bin_paths(t_ms *ms, char **env)
{
	(void)ms;
	int		i;
	char	*env_path;
	char	**binaries;

	i = 0;
	env_path = NULL;
	while (env && env[i])
	{
		if (ft_strncmp("PATH", env[i], 4) == 0)
		{
			env_path = ft_strdup(env[i] + 5);// a proteger?
			break ;
		}
		i++;
	}
	if (!env_path)
		env_path = get_bin_path_underscore(env);
	binaries = ft_split(env_path, ':');
	if (!binaries)
		return (free(env_path), NULL);// a proteger autrement
	free(env_path);
	return (binaries);
}

int	build_path(t_cmd *cmd)
{
//	dprintf(2, "args[0] = %s\n", cmd->args[0]);
	char	*path;
	int		i;

	i = 0;
	while (cmd->bin_paths && cmd->bin_paths[i])
	{
		path = ft_strjoin_slash(cmd->bin_paths[i], cmd->args[0]);
		if (!path)
			return (1);
		if (access(path, F_OK | X_OK) == 0)
		{
			free(cmd->args[0]);
			cmd->args[0] = path;
			cmd->valid_path = 1;
			return (0);
		}
		free(path);
		i++;
	}
	return (0);
}
