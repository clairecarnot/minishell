/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 10:12:58 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/30 16:17:57 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/export.h"
#include "../libft/libft.h"

/*
Ajouter a la liste env, a la fin
*/
void	add_to_env(t_ms *ms, char *content)
{
	t_list	*new;

	printf("content2 = %s\n", content);
	new = ft_lstnew(content);//ajouter protec
	ft_lstadd_back(&ms->env, new);
}

void	print_lst_env(t_list *env)
{
	t_list	*lst;

	lst = env;
	while (lst)
	{
		printf("%s\n", (char *)lst->content);
		lst = lst->next;
	}
}

/*
La commande env liste les variables environnment 
*/
int	exec_env(t_ms *ms)
{
	t_list	*exp_arg;
	
	exp_arg = ms->root->args;
	if (ft_strncmp(exp_arg->content, "env", 5) == 0)
	{
		print_lst_env(ms->env);
	}
	return (0);
}
