/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_join_exp.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 16:45:56 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/31 11:03:23 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"
#include "../libft/libft.h"

char	*sjoin_noequal(char *s1, char *s2, int i)
{
	char	*dest;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	dest = malloc(sizeof(char) * ft_strlen(s1) + ft_slen(s2) + 1 + 3);// c'est verifie 2
	if (!dest)
		return (NULL);
	while (s1[++i])
		dest[i] = s1[i];
	dest[i++] = '=';
	dest[i++] = '"';
	j = 0;
	if (s2)
	{
		while (s2[j])
		{
			dest[i + j] = s2[j];
			j++;
		}
	}
	dest[i + j] = '"';
	dest[i + j + 1] = '\0';
	return (dest);
}

char	*sjoin_wquote(char *s1, char *s2, int i)
{
	char	*dest;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	dest = malloc(sizeof(char) * ft_strlen(s1) + ft_slen(s2) + 1);// c'est verifie 2
	if (!dest)
		return (NULL);
	while (i < (int)(ft_strlen(s1) - 1))
	{
		dest[i] = s1[i];
		i++;
	}
	j = 0;
	if (s2)
	{
		while (s2[j])
		{
			dest[i + j] = s2[j];
			j++;
		}
	}
	dest[i + j] = '"';
	dest[i + j + 1] = '\0';
	return (dest);
}

t_list	*join_in_exp3(t_ms *ms, char *cpy_ct, char *join_ct, t_cmd *cmd)
{
	t_list	*new;

	free(cpy_ct);
	new = ft_lstnew(join_ct);// c'est verifie 2
	if (!new)
	{
		free_cmd(cmd);
		prefree_minishell(ms, join_ct);
	}
	return (new);
}

char	*join_in_exp2(t_ms *ms, t_list *exp_tmp, char *cpy_ct, t_cmd *cmd)
{
	char	*join_ct;

	if (has_equal(exp_tmp->content))
		join_ct = sjoin_wquote(exp_tmp->content, cpy_ct, 0);// c'est protege 2
	else
		join_ct = sjoin_noequal(exp_tmp->content, cpy_ct, -1);// c'est protege 2
	if (!join_ct)
	{
		free(cpy_ct);
		prefree_minishell_cmd(ms, cmd);
	}
	return (join_ct);
}

void	join_in_exp(t_ms *ms, t_cmd *cmd, char *content)
{
	char	*cpy_ct;
	char	*join_ct;
	t_list	*exp_tmp;
	t_list	*exp_tmp2;
	t_list	*new;

	cpy_ct = dup_after_equal(ms, cmd, content, 0);// c'est protege 2
	exp_tmp = ms->exp;
	exp_tmp2 = ms->exp;
	while (exp_tmp)
	{
		if (ft_strncmp(exp_tmp->content, content, slen_equal(content)) == 0)
		{
			join_ct = join_in_exp2(ms, exp_tmp, cpy_ct, cmd);
			new = join_in_exp3(ms, cpy_ct, join_ct, cmd);
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
