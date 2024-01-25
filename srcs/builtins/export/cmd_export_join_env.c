/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_join_env.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:05:34 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/25 11:47:44 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../libft/libft.h"

char	*dup_after_equal(t_ms *ms, t_cmd *cmd, char *s, int i)
{
	char	*dest;
	int		j;

	while (s[i] && s[i] != '=')
		i++;
	i++;
	if (s[i] == '\0')
		return (NULL);
	dest = malloc(sizeof(char) * (ft_strlen(s) - (slen_equal(s) - 2) + 1));//verifie 2
	if (!dest)
		prefree_minishell_cmd(ms, cmd);
	i = 0;
	j = 0;
	while (s[i] && s[i] != '+')
		i++;
	i += 2;
	while (s[i])
		dest[j++] = s[i++];
	dest[j] = '\0';
	return (dest);
}

char	*ft_sjoin(t_ms *ms, char *s1, char *s2, t_cmd *cmd)
{
	char	*dest;
	int		i;
	int		j;

	i = -1;
	dest = malloc(sizeof(char) * ft_strlen(s1) + ft_slen(s2) + 1);// c'est verifie 2
	if (!dest)
	{
		if (s2)
			free(s2);
		prefree_minishell_cmd(ms, cmd);
	}
	while (s1[++i])
		dest[i] = s1[i];
	j = 0;
	if (s2)
	{
		while (s2[j])
		{
			dest[i + j] = s2[j];
			j++;
		}
	}
	dest[i + j] = '\0';
	return (dest);
}

t_list	*join_in_env2(t_ms *ms, char *cpy_ct, char *join_ct, t_cmd *cmd)
{
	t_list	*new;

	free(cpy_ct);
	new = ft_lstnew(join_ct);// c'est verifie 2
	if (!new)
	{
		free(join_ct);
		prefree_minishell_cmd(ms, cmd);
	}
	return (new);
}

void	join_in_env(t_ms *ms, t_cmd *cmd, char *content)
{
	char	*cpy_ct;
	char	*join_ct;
	t_list	*env_tmp;
	t_list	*env_tmp2;
	t_list	*new;

	cpy_ct = dup_after_equal(ms, cmd, content, 0);// c'est verifie 1
	env_tmp = ms->env;
	env_tmp2 = ms->env;
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->content, content, slen_equal(content)) == 0)
		{
			join_ct = ft_sjoin(ms, env_tmp->content, cpy_ct, cmd);
			new = join_in_env2(ms, cpy_ct, join_ct, cmd);
			new->next = env_tmp->next;
			free(env_tmp->content);
			free(env_tmp);
			env_tmp2->next = new;
			return ;
		}
		env_tmp2 = env_tmp;
		env_tmp = env_tmp->next;
	}
}
