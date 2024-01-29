/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_replace.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:05:12 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/26 14:07:54 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"
#include "../libft/libft.h"

char	*ft_sdup(t_ms *ms, t_cmd *cmd, char *s)
{
	char	*dest;
	int		i;

	dest = malloc(sizeof(char) * ft_strlen(s) + 1);// c'est verifie 2
	if (!dest)
	{
		free_cmd(cmd);
		ms->exit_code = 255;
		free_minishell(ms, 1);
	}
	i = 0;
	while (((char *)s)[i])
	{
		dest[i] = ((char *)s)[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/*
Remplace dans la liste exp, le contenu d'une variable deja existante
*/
void	replace_in_exp(t_ms *ms, t_cmd *cmd, char *content)
{
	t_list	*exp_tmp;
	t_list	*exp_tmp2;
	t_list	*new;
	char	*cpy_ct;

	exp_tmp = ms->exp;
	exp_tmp2 = ms->exp;
	cpy_ct = add_qvar(content, 0);
	if (!cpy_ct)
		(free_cmd(cmd), prefree_minishell(ms, NULL));// c'est verifie 2
	new = ft_lstnew(cpy_ct);// c'est verifie 2
	if (!new)
		(free_cmd(cmd), prefree_minishell(ms, cpy_ct));
	while (exp_tmp)
	{
		if (ft_strncmp(exp_tmp->content, cpy_ct, slen_equal(cpy_ct)) == 0)
		{
			new->next = exp_tmp->next;
			(free(exp_tmp->content), free(exp_tmp));
			exp_tmp2->next = new;
			return ;
		}
		exp_tmp2 = exp_tmp;
		exp_tmp = exp_tmp->next;
	}
}

void	replace_in_env(t_ms *ms, t_cmd *cmd, char *ct)
{
	char	*cpy_content;
	t_list	*env_tmp;
	t_list	*env_tmp2;
	t_list	*new;

	cpy_content = ft_sdup(ms, cmd, ct);// c'est protege 2
	env_tmp = ms->env;
	env_tmp2 = ms->env;
	new = ft_lstnew(cpy_content);
	if (!new)
		(free_cmd(cmd), prefree_minishell(ms, cpy_content));// c'est verifie 2
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->content, ct, slen_equal(ct)) == 0 && \
		ft_strncmp(env_tmp->content, ct, slen_equal(env_tmp->content)) == 0)
		{
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
