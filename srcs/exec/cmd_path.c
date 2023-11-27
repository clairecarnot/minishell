#include "../../include/exec.h"

void	abs_rel_path(t_cmd *cmd)
{
	cmd->abs_or_rel = 1;
	if (access(cmd->args[0], F_OK | X_OK) == 0)
		cmd->valid_path = 1;
	cmd->builtin = NOBUILT;
}

char	**get_bin_paths(char **env)
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
			break ;
		}
		i++;
	}
	if (!env_path)
		return (NULL);
	binaries = ft_split(env_path, ':');
	if (!binaries)
		return (free(env_path), NULL);
	free(env_path);
	return (binaries);
}

void	build_path(t_cmd *cmd)
{
	char	*path;
	int		i;

	i = 0;
	while (cmd->bin_paths && cmd->bin_paths[i])
	{
		path = ft_strjoin_slash(cmd->bin_paths[i], cmd->args[0]);
		if (!path)
			return ;
		if (access(path, F_OK | X_OK) == 0)
		{
			free(cmd->args[0]);
			cmd->args[0] = path;
			cmd->valid_path = 1;
			return ;
		}
		free(path);
		i++;
	}
}
