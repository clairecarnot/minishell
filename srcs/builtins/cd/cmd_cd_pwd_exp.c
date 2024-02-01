/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd_pwd_exp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:20:43 by mapoirie          #+#    #+#             */
/*   Updated: 2024/02/01 17:20:45 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"
#include "../libft/libft.h"

void	replace_oldpwd_exp(t_ms *ms, t_cmd *cmd)
{
	char	*content;
	char	*content_qt;
	t_list	*exp_tmp;

	exp_tmp = ms->exp;
	content = ft_strjoin("OLDPWD=", ms->wkdir);
	if (!content)
	{
		free_cmd(cmd);
		ms->exit_code = 255;
		free_minishell(ms, 1);
	}
	content_qt = add_qvar_pwd(ms, cmd, content, 0);
	free(content);
	while (exp_tmp)
	{
		if (ft_strncmp(exp_tmp->content, "OLDPWD", 6) == 0)
		{
			free(exp_tmp->content);
			exp_tmp->content = content_qt;
			return ;
		}
		exp_tmp = exp_tmp->next;
	}
	free(content_qt);
}

char	*add_qvar_pwd(t_ms *ms, t_cmd *cmd, char *content, int i)
{
	int		j;
	char	*content_qt;

	j = 0;
	content_qt = malloc((ft_strlen(content) + 3) * sizeof(char));
	if (!content_qt)
	{
		free(content);
		free_cmd(cmd);
		ms->exit_code = 255;
		free_minishell(ms, 1);
	}
	while (content[i] && content[i] != '=')
		content_qt[j++] = content[i++];
	i++;
	content_qt[j++] = '=';
	content_qt[j++] = '\"';
	while (content[i])
		content_qt[j++] = content[i++];
	content_qt[j++] = '\"';
	content_qt[j] = '\0';
	return (content_qt);
}

void	preprefree_minishell(t_ms *ms, t_cmd *cmd)
{
	free_cmd(cmd);
	prefree_minishell(ms, NULL);
}

void	replace_pwd_exp(t_ms *ms, t_cmd *cmd)
{
	char	*content;
	char	path[1024];
	char	*content_qt;
	t_list	*exp_tmp;

	exp_tmp = ms->exp;
	if (getcwd(path, sizeof(path)) != NULL)
	{
		content = ft_strjoin("PWD=", path);
		if (!content)
			(free_cmd(cmd), prefree_minishell(ms, NULL));
		content_qt = add_qvar_pwd(ms, cmd, content, 0);
		free(content);
		while (exp_tmp)
		{
			if (ft_strncmp(exp_tmp->content, "PWD", 3) == 0)
			{
				free(exp_tmp->content);
				exp_tmp->content = content_qt;
				return ;
			}
			exp_tmp = exp_tmp->next;
		}
		free(content_qt);
	}
}

void	replace_pwd_env_exp(t_ms *ms, t_cmd *cmd)
{
	replace_oldpwd_env(ms, cmd);
	replace_oldpwd_exp(ms, cmd);
	replace_pwd_env(ms, cmd);
	replace_pwd_exp(ms, cmd);
}
