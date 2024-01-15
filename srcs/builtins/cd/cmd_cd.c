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
			return(get_dir(ms, cmd, env_tmp->content));// a verife si ca marche
		env_tmp = env_tmp->next;
	}
	return (NULL);
}

int	exec_cd(t_ms *ms, t_cmd *cmd)
{
	char	*tmp_dir;
	char	*tmp_dir2;
	char	*dir;

	if (cmd->args && cmd->args[1] && cmd->args[2])// cd avec deux args --> erreur
	{
		ms->exit_code = 1;
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	}
	if (cmd->args && !cmd->args[1])// cd alone --> doit aller dans HOME
	{
		dir = getvar_env(ms, cmd, "HOME");
		chdir(dir);
		return (free(dir), 1);
	}
	if (cmd->args && cmd->args[1])// cd chemin
	{
		if (cmd->args[1][0] != '/')
		{
			tmp_dir = getvar_env(ms, cmd, "PWD");
			tmp_dir2 = ft_strjoin(tmp_dir, "/");
			dir = ft_strjoin(tmp_dir2, cmd->args[1]);
			free(tmp_dir2);
			free(tmp_dir);
			if (chdir(dir) != 0)
			{
				free(dir);
				perror("cd");
				// ft_putstr_fd("cd: ", 2);
				// ft_putstr_fd(cmd->args[1], 2);
				// ft_putstr_fd(": No such file or directory\n", 2);
			}
			else
			{
				free(dir);
				replace_oldpwd_env(ms, cmd);
				replace_oldpwd_exp(ms, cmd);
				replace_pwd_env(ms, cmd);
				replace_pwd_exp(ms, cmd);
			}
			return (1);
		}
		else
		{
			if (chdir(cmd->args[1]) != 0)
			{
				ft_putstr_fd("bash: cd: ", 2);
				ft_putstr_fd(cmd->args[1], 2);
				ft_putstr_fd(": No such file or directory\n", 2);
			}
			else
			{
				replace_oldpwd_env(ms, cmd);
				replace_oldpwd_exp(ms, cmd);
				replace_pwd_env(ms, cmd);
				replace_pwd_exp(ms, cmd);
			}
			return (1);
		}
	}
	return (0);
}
