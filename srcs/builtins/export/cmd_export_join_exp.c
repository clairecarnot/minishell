/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_join_exp.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 16:45:56 by mapoirie          #+#    #+#             */
/*   Updated: 2023/12/12 16:47:25 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../libft/libft.h"

char	*sjoin_noequal(t_ms *ms, char *s1, char *s2, int i)
{
	char	*dest;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	dest = malloc(sizeof(char) * ft_strlen(s1) + ft_slen(s2) + 1 + 3);// + 3 pour les quotes et le egale
	if (!dest)
		prefree_minishell(ms, s2);
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

char	*sjoin_wquote(t_ms *ms, char *s1, char *s2, int i)
{
	char	*dest;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	dest = malloc(sizeof(char) * ft_strlen(s1) + ft_slen(s2) + 1);
	if (!dest)
		prefree_minishell(ms, s2);
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

t_list	*join_in_exp3(t_ms *ms, char *cpy_ct, char *join_ct)
{
	t_list	*new;

	free(cpy_ct);
	new = ft_lstnew(join_ct);// c'est protege
	if (!new)
		prefree_minishell(ms, join_ct);
	return (new);
}

char	*join_in_exp2(t_ms *ms, t_list *exp_tmp, char *cpy_ct)
{
	char	*join_ct;
	
	if (has_equal(exp_tmp->content))
		join_ct = sjoin_wquote(ms, exp_tmp->content, cpy_ct, 0);// c'est protege
	else
		join_ct = sjoin_noequal(ms, exp_tmp->content, cpy_ct, -1);// c'est protege	
	return (join_ct);
}

void	join_in_exp(t_ms *ms, char *content)
{
	char	*cpy_ct;
	char	*join_ct;
	t_list	*exp_tmp;
	t_list	*exp_tmp2;
	t_list	*new;

	cpy_ct = dup_after_equal(ms, content, 0);// c'est protege
	exp_tmp = ms->exp;
	exp_tmp2 = ms->exp;
	while (exp_tmp)
	{
		if (ft_strncmp(exp_tmp->content, content, slen_equal(content)) == 0)
		{
			join_ct = join_in_exp2(ms, exp_tmp, cpy_ct);
			new = join_in_exp3(ms, cpy_ct, join_ct);
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
