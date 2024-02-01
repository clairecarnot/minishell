/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd_pwd_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:21:01 by mapoirie          #+#    #+#             */
/*   Updated: 2024/02/01 17:21:02 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"
#include "../libft/libft.h"

void	replace_oldpwd_env(t_ms *ms, t_cmd *cmd)
{
	char	*content;
	t_list	*env_tmp;

	env_tmp = ms->env;
	content = ft_strjoin("OLDPWD=", ms->wkdir);
	if (!content)
	{
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
			return ;
		}
		env_tmp = env_tmp->next;
	}
	free(content);
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
		if (!content)
		{
			free_cmd(cmd);
			prefree_minishell(ms, NULL);
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
