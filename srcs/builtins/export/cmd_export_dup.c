/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_dup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:05:34 by mapoirie          #+#    #+#             */
/*   Updated: 2023/12/11 17:54:50 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../libft/libft.h"


char	*ft_strjoin_noequal(char *s1, char *s2)
{
	char	*dest;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	// if (!s1)
	// 	return (s2);
	i = -1;
	dprintf(2, "strjoin_noequal\n");
	dest = malloc(sizeof(char) * ft_strlen(s1) + ft_slen(s2) + 1 + 3);// + 3 pour les quotes et le egale
	if (!dest)
		return (NULL);
	dprintf(2, "strjoin_noequal0\n");
	while (s1[++i])
		dest[i] = s1[i];
	dprintf(2, "strjoin_noequal1\n");
	dest[i++] = '=';
	dest[i++] = '"';
	j = 0;
	dprintf(2, "strjoin_noequal2\n");
	if (s2)
	{
		
		while (s2[j++])
			dest[i + j] = s2[j];
	}
	dprintf(2, "strjoin_noequal3\n");
	dest[i + j] = '"';
	dest[i + j + 1] = '\0';
	dprintf(2, "strjoin_noequal4\n");
	return (dest);
}

char	*ft_strjoin_wquote(char *s1, char *s2)
{
	char	*dest;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	// if (!s1)
	// 	return (s2);
	// if (!s2)
	// 	return (s1);
	dest = malloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!dest)
		return (0x0);
	i = 0;
	while (i < (int)(ft_strlen(s1) - 1))
	{
		dest[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		dest[i + j] = s2[j];
		j++;
	}
	dest[i + j] = '"';
	dest[i + j + 1] = '\0';
	return (dest);
}

char	*dup_after_equal(t_ms *ms, char *s, int i)
{
	char	*dest;
	int		j;

	while (s[i] && s[i] != '=')
		i++;
	i++;
	if(s[i] == '\0')
		return (NULL);
	dest = malloc(sizeof(char) * (ft_strlen(s) - (ft_strlen_equal(s) - 2) + 1));// c'est verifie
	if (!dest)
	{
		ms->exit_code = 134;
		free_minishell(ms, 1);
	}
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
char	*ft_sjoin(t_ms *ms, char *s1, char *s2)
{
	char	*dest;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	dest = malloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 1);//ft_slen
	if (!dest)
	{
		ms->exit_code = 134;
		free_minishell(ms, 1);
	}
	i = -1;
	while (s1[++i])
		dest[i] = s1[i];
	j = -1;
	while (s2[++j])
		dest[i + j] = s2[j];
	dest[i + j] = '\0';
	return (dest);
}

void	join_in_exp(t_ms *ms, char *content)
{
	char	*cpy_content;
	char	*join_content;
	t_list	*exp_tmp;
	t_list	*exp_tmp2;
	t_list	*new;

	cpy_content = dup_after_equal(ms, content, 0);//ajout protec
	dprintf(2, "cpy_content = %s\n", cpy_content);
	exp_tmp = ms->exp;
	exp_tmp2 = ms->exp;
	while (exp_tmp)
	{
		if (ft_strncmp(exp_tmp->content, content, ft_strlen_equal(content)) == 0)
		{
			if (has_equal(exp_tmp->content))
				join_content = ft_strjoin_wquote(exp_tmp->content, cpy_content);// a proteger
			else
				join_content = ft_strjoin_noequal(exp_tmp->content, cpy_content);
			free(cpy_content);
			new = ft_lstnew(join_content);// a proteger ?
			new->next = exp_tmp->next;
			free(exp_tmp->content);
			free(exp_tmp);
			exp_tmp2->next = new;
			return ;
		}
		exp_tmp2 = exp_tmp;
		exp_tmp = exp_tmp->next;
	}
}



void	join_in_env(t_ms *ms, char *content)
{
	char	*cpy_content;
	char	*join_content;
	t_list	*env_tmp;
	t_list	*env_tmp2;
	t_list	*new;

	cpy_content = dup_after_equal(ms, content, 0);// c'est protege
	env_tmp = ms->env;
	env_tmp2 = ms->env;
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->content, content, ft_strlen_equal(content)) == 0)
		{
			join_content = ft_sjoin(ms, env_tmp->content, cpy_content);// a proteger
			// checker lorsque cpy_content est null
			free(cpy_content);
			new = ft_lstnew(join_content);// a proteger ?
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
