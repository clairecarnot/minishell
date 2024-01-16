#include "../../../include/builtins.h"
#include "../libft/libft.h"

void	cd_alone(t_ms *ms, t_cmd *cmd)
{
	char	*home_var;

	home_var = getvar_env(ms, cmd, "HOME");
	if (ms->home && home_var)
	{
		free(home_var);
		if (chdir(ms->home))// c'est verife
		{
			perror("chdir");
			free_cmd(cmd);
			prefree_minishell(ms, NULL);
		}
		// chdir(ms->home);
		replace_oldpwd_env(ms, cmd);
		replace_oldpwd_exp(ms, cmd);
		replace_pwd_env(ms, cmd);
		replace_pwd_exp(ms, cmd);
	}
	else if (!home_var)
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
}

int	cd_dash(t_ms *ms, t_cmd *cmd)
{
	t_list	*env_tmp;

	env_tmp = ms->env;
	if (cmd->args[1][1])
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
	}
	else
	{
		while (env_tmp)
		{
			if (ft_strncmp(env_tmp->content, "OLDPWD", 6) == 0)
			{
				printf("%s\n", (char *)env_tmp->content + 7);
				return (chdir(env_tmp->content + 7), 0);
			}
			env_tmp = env_tmp->next;
		}
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
	}
	return (0);
}

void	cd_tilde(t_ms *ms, t_cmd *cmd)
{
	(void)cmd;
	if (ms->home)
		chdir(ms->home);
}

void	cd_slash(t_ms *ms, t_cmd *cmd)
{
	if (chdir(cmd->args[1]) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
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
}

int	exec_cd(t_ms *ms, t_cmd *cmd)
{
	if (cmd->args && cmd->args[1] && cmd->args[2])
	{
		ms->exit_code = 1;
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	}
	if (cmd->args && !cmd->args[1])
		cd_alone(ms, cmd);
	if (cmd->args && cmd->args[1])
	{
		if (cmd->args[1][0] == '-')
			cd_dash(ms, cmd);
		else if (cmd->args[1][0] == '~' && !cmd->args[1][1])
			cd_tilde(ms, cmd);
		else if (cmd->args[1][0] == '/')
			cd_slash(ms, cmd);
		else if (cmd->args[1][0] != '/')
			cd_else(ms, cmd);
	}
	return (0);
}
