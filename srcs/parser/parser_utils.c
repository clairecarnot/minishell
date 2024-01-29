/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:52:28 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/29 15:29:18 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

/*
 * new_node:
 * Cree et initialise une structure AST du type indique
 */

t_ast	*new_node(t_ms *ms, t_node_type type)
{
	t_ast	*new_ast;

	new_ast = ft_calloc(1, sizeof(t_ast));
	if (!new_ast)
	{
		ms->exit_code = 255;
		return (NULL);
	}
	new_ast->type = type;
	new_ast->right = NULL;
	new_ast->left = NULL;
	new_ast->args = NULL;
	new_ast->subsh = 0;
	new_ast->redirs = NULL;
	new_ast->dol = NULL;
	new_ast->wil = NULL;
	new_ast->pipe = NULL;
	new_ast->parent = NULL;
	return (new_ast);
}

/*
 * Convertit un type de token en un type d'ast
 */

int	token_to_node(int type)
{
	if (type == T_WORD)
		return (CMD);
	else if (type == T_PIPE)
		return (PIPE);
	else if (type == T_AND_IF)
		return (AND_IF);
	else if (type == T_OR_IF)
		return (OR_IF);
	else if (type == T_LESS)
		return (LESS);
	else if (type == T_GREAT)
		return (GREAT);
	else if (type == T_DLESS)
		return (DLESS);
	else if (type == T_DGREAT)
		return (DGREAT);
	else
		return (-1);
}

/*
 * eat_token:
 * "Mange" un token du type indique => fait
 *  avancer une liste de token au token suivant
 */

int	eat_token(t_ms *ms, t_type type)
{
	if (ms->cur_tok->type == type)
	{
		ms->cur_tok = ms->cur_tok->next_token;
		return (1);
	}
	return (0);
}

t_list	*lst_dup_int(t_list *lst_tocpy)
{
	t_list	*new;
	t_list	*tmp;
	t_list	*lst;

	new = NULL;
	tmp = NULL;
	if (!lst_tocpy)
		return (NULL);
	lst = lst_tocpy;
	while (lst)
	{
		tmp = ft_lstnew_int(lst->n);
		if (!tmp)
			return (ft_lstfree(&new), NULL);
		ft_lstadd_back(&new, tmp);
		lst = lst->next;
	}
	return (new);
}
