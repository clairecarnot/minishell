/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_join_env.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:05:34 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/31 16:05:44 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"
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

//unset _ SHLVL PWD OLDPWD LD_LIBRARY_PATH  GLIBCPP_FORCE_NEW GLIBCXX_FORCE_NEW LD_PRELOAD
//unset GLIBCPP_FORCE_NEW GLIBCXX_FORCE_NEW LD_PRELOAD
void	join_in_env(t_ms *ms, t_cmd *cmd, char *content)
{
	char	*cpy_ct;
	char	*join_ct;
	t_list	*env_tmp;

	cpy_ct = dup_after_equal(ms, cmd, content, 0);// c'est verifie 1
	env_tmp = ms->env;
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->content, content, slen_equal(content)) == 0)
		{
			join_ct = ft_sjoin(ms, env_tmp->content, cpy_ct, cmd);
			free(cpy_ct);
			free(env_tmp->content);
			env_tmp->content = join_ct;
			return ;
		}
		env_tmp = env_tmp->next;
	}
}
