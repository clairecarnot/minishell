/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:27:37 by mapoirie          #+#    #+#             */
/*   Updated: 2024/02/01 17:27:39 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"
#include "../libft/libft.h"

void	del_var_exp(t_ms *ms, char *content)
{
	t_list	*cur;
	t_list	*prev;

	cur = ms->exp;
	prev = NULL;
	while (cur && (ft_strncmp(cur->content, content, slen_equal(cur->content)) \
	|| ft_strncmp(cur->content, content, ft_strlen(content))))
	{
		prev = cur;
		cur = cur->next;
	}
	if (cur == NULL)
		return ;
	if (prev != NULL)
		prev->next = cur->next;
	else
		ms->exp = cur->next;
	free(cur->content);
	free(cur);
}

void	del_var_env(t_ms *ms, char *content)
{
	t_list	*cur;
	t_list	*prev;

	cur = ms->env;
	prev = NULL;
	while (cur && (ft_strncmp(cur->content, content, slen_equal(cur->content)) \
	|| ft_strncmp(cur->content, content, ft_strlen(content))))
	{
		prev = cur;
		cur = cur->next;
	}
	if (cur == NULL)
		return ;
	if (prev != NULL)
		prev->next = cur->next;
	else
		ms->env = cur->next;
	free(cur->content);
	free(cur);
}

int	exec_unset(t_ms *ms, t_cmd *cmd)
{
	int	i;

	i = 1;
	if (!cmd->args[i])
		return (0);
	else
	{
		while (cmd->args[i])
		{
			del_var_env(ms, cmd->args[i]);
			del_var_exp(ms, cmd->args[i]);
			i++;
			if (!cmd->args[i])
				return (0);
		}
	}
	return (0);
}
