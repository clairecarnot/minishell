#include "../../../include/builtins.h"
#include "../libft/libft.h"

char	*get_dir(t_ms *ms, t_cmd *cmd, char *var_line)
{
	int		i;
	int		j;
	int		size;
	char	*dir;

	i = 0;
	j = 0;
	size = 0;
	while (var_line[i] && var_line[i] != '=')
		i++;
	i++;
	while (var_line[i + size])
		size++;
	dir = malloc(sizeof(char) * (size + 1));// c'est verifie
	if (!dir)
	{
		free_cmd(cmd);
		ms->exit_code = 255;
		free_minishell(ms, 1);
	}
	while (var_line[i])
		dir[j++] = var_line[i++];
	dir[j] = '\0';
	return (dir);
}

char	*getvar_env(t_ms *ms, t_cmd *cmd, char *var_name)
{
	t_list *env_tmp;

	env_tmp = ms->env;
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->content, var_name, ft_strlen(var_name)) == 0)
		{
			return(get_dir(ms, cmd, env_tmp->content));// a verife si ca marche
		}
		env_tmp = env_tmp->next;
	}
	return (NULL);
}

void	nosuchfile_cd(char *str)
{
	ft_putstr_fd("minishell: cd : ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

void	cd_else(t_ms *ms, t_cmd *cmd)
{
	char	*tmp_dir;
	char	*dir;

	if (!ms->wkdir)
		nosuchfile_cd(cmd->args[1]);
	else
	{	
		tmp_dir = ft_strjoin(ms->wkdir, "/");
		if (!tmp_dir)
			preprefree_minishell(ms, cmd);
		dir = ft_strjoin(tmp_dir, cmd->args[1]);
		free(tmp_dir);
		if (!dir)
			preprefree_minishell(ms, cmd);
		if (chdir(dir) != 0)
		{
			free(dir);
			nosuchfile_cd(cmd->args[1]);
		}
		else
		{
			free(dir);
			replace_pwd_env_exp(ms, cmd);
		}
	}
}
