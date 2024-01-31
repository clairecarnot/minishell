/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_replace.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:05:12 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/31 16:04:16 by mapoirie         ###   ########.fr       */
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
	char	*cpy_ct;

	exp_tmp = ms->exp;
	cpy_ct = add_qvar(content, 0);
	if (!cpy_ct)
		(free_cmd(cmd), prefree_minishell(ms, NULL));// c'est verifie 2
	while (exp_tmp)
	{
		if (ft_strncmp(exp_tmp->content, cpy_ct, slen_equal(cpy_ct)) == 0)
		{
			free(exp_tmp->content);
			exp_tmp->content = cpy_ct;
			return ;
		}
		exp_tmp = exp_tmp->next;
	}
}

void	replace_in_env(t_ms *ms, t_cmd *cmd, char *ct)
{
	char	*cpy_content;
	t_list	*env_tmp;

	cpy_content = ft_sdup(ms, cmd, ct);// c'est protege 2
	env_tmp = ms->env;
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->content, ct, slen_equal(ct)) == 0)
		{
			free(env_tmp->content);
			env_tmp->content = cpy_content;
			return ;
		}
		env_tmp = env_tmp->next;
	}
}
