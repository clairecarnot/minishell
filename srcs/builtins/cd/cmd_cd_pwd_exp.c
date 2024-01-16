#include "../../../include/builtins.h"
#include "../libft/libft.h"

// char	*replace_oldpwd_exp2(t_ms *ms, t_cmd *cmd)
// {
// 	char	*content_tmp;
// 	t_list	*exp_tmp;

// 	exp_tmp = ms->exp;
// 	while (exp_tmp)
// 	{
// 		if (ft_strncmp(exp_tmp->content, "PWD", 3) == 0)
// 		{
// 			content_tmp = ft_strdup(exp_tmp->content + 4);
// 			if (!content_tmp)// a verifier
// 			{
// 				free_cmd(cmd);
// 				ms->exit_code = 255;
// 				free_minishell(ms, 1);
// 			}
// 			return (content_tmp);
// 		}
// 		exp_tmp = exp_tmp->next;
// 	}
// 	return (NULL);
// }

void	replace_oldpwd_exp(t_ms *ms, t_cmd *cmd)
{
	// char	*content_tmp;
	char	*content;
	char	*content_qt;
	t_list	*exp_tmp;

	exp_tmp = ms->exp;
	// content_tmp = replace_oldpwd_exp2(ms, cmd);
	
	content = ft_strjoin("OLDPWD=", ms->wkdir);
	if (!content)
	{
		// free(content_tmp);
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
			return;
		}
		exp_tmp = exp_tmp->next;
	}
	free(content_qt);
}

char	*add_qvar_pwd(t_ms *ms, t_cmd* cmd, char *content, int i)
{
	int		j;
	char	*content_qt;

	j = 0;
	content_qt = malloc((ft_strlen(content) + 3) * sizeof(char));
	if (!content_qt)// a verifier
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

void	replace_pwd_exp(t_ms *ms, t_cmd *cmd)
{
	char	path[1024];
	char	*content;
	char	*content_qt;
	t_list	*exp_tmp;

	exp_tmp = ms->exp;
	if (getcwd(path, sizeof(path)) != NULL)
	{
		content = ft_strjoin("PWD=", path);
		if (!content)// a verifier
		{
			free_cmd(cmd);
			ms->exit_code = 255;
			free_minishell(ms, 1);
		}
		content_qt = add_qvar_pwd(ms, cmd, content, 0);
		// dprintf(2, "content_qt = %s\n", content_qt);
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

