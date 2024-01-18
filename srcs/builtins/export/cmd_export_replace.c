/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_replace.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 12:05:12 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/18 12:01:23 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../libft/libft.h"

char	*ft_sdup(t_ms *ms, char *s)
{
	char	*dest;
	int		i;

	dest = malloc(sizeof(char) * ft_strlen(s) + 1);// c'est protege
	if (!dest)//verifier que ft_strlen ne crash pas quand s est null
	{
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
	char	*cpy_content;

	(void)cmd;
	exp_tmp = ms->exp;
	exp_tmp2 = ms->exp;
	cpy_content = add_qvar(ms, content, 0, 0 /* ou 1 s'il faut free content*/);
	new = ft_lstnew(cpy_content);// a proteger
	while (exp_tmp)
	{
		if (ft_strncmp(exp_tmp->content, cpy_content, slen_equal(cpy_content)) == 0)
		{
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

void	replace_in_env(t_ms *ms, t_cmd *cmd, char *content)
{
	char	*cpy_content;
	t_list	*env_tmp;
	t_list	*env_tmp2;
	t_list	*new;

	(void)cmd;
	cpy_content = ft_sdup(ms, content);// c'est protege
	env_tmp = ms->env;
	env_tmp2 = ms->env;
	new = ft_lstnew(cpy_content);
	if (!new)
		prefree_minishell(ms, cpy_content);
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->content, content, slen_equal(content)) == 0 &&\
		ft_strncmp(env_tmp->content, content, slen_equal(env_tmp->content)) == 0)
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
