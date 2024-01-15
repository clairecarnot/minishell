#include "../../../include/builtins.h"
#include "../libft/libft.h"

// char	*replace_oldpwd_env2(t_ms *ms, t_cmd *cmd)
// {
// 	char	*content_tmp;
// 	t_list	*env_tmp;

// 	env_tmp = ms->env;
// 	while (env_tmp)
// 	{
// 		if (ft_strncmp(env_tmp->content, "PWD", 3) == 0)
// 		{
// 			content_tmp = ft_strdup(env_tmp->content + 4);
// 			if (!content_tmp)// a verifier
// 			{
// 				free_cmd(cmd);
// 				ms->exit_code = 255;
// 				free_minishell(ms, 1);
// 			}
// 			return (content_tmp);
// 		}
// 		env_tmp = env_tmp->next;
// 	}
// 	return (NULL);
// }

void	replace_oldpwd_env(t_ms *ms, t_cmd *cmd)
{
	// char	*content_tmp;
	char	*content;
	t_list	*env_tmp;

	env_tmp = ms->env;
	// content_tmp = replace_oldpwd_env2(ms, cmd);
	content = ft_strjoin("OLDPWD=", ms->wkdir);
	if (!content)
	{
		// free(content_tmp);
		free_cmd(cmd);
		ms->exit_code = 255;
		free_minishell(ms, 1);
	}
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->content, "OLDPWD", 6) == 0)
		{
			free(env_tmp->content);
			env_tmp->content = content;
			return;
		}
		env_tmp = env_tmp->next;
	}
}

void	replace_pwd_env(t_ms *ms, t_cmd *cmd)
{
	char	path[1024];
	char	*content;
	t_list	*env_tmp;

	env_tmp = ms->env;
	if (getcwd(path, sizeof(path)) != NULL)
	{
		content = ft_strjoin("PWD=", path);
		if (!content)// a verifier
		{
			free_cmd(cmd);
			ms->exit_code = 255;
			free_minishell(ms, 1);
		}
		while (env_tmp)
		{
			if (ft_strncmp(env_tmp->content, "PWD", 3) == 0)
			{
				free(env_tmp->content);
				env_tmp->content = content;
				return ;
			}
			env_tmp = env_tmp->next;
		}
		free(content);
	}
}
