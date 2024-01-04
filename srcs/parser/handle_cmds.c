/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:20:17 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/04 15:20:54 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

void	ft_doladd_back(t_dol **dol, t_dol *new)
{
	t_list	*ptr_c;
	t_list	*ptr_d;

	if (*dol == NULL)
		*dol = new;
	else
	{
		ptr_d = (*dol)->d;
		ptr_c = (*dol)->c;
		while (ptr_d->next)
			ptr_d = ptr_d->next;
		ptr_d->next = new->d;
		while (ptr_c->next)
			ptr_c = ptr_c->next;
		ptr_c->next = new->c;
		free(new);
	}
}

/*
 * add_cmd_args:
 * Cree une nouvelle structure t_list qui a pour content la valeur de la commande ou de l'argument qu'on ajoute a la liste d'arguments de l'AST
 */

t_list	*add_cmd_args(t_ms *ms, t_ast *new_ast)
{
//	dprintf(2, "add cmd args\n");
//	dprintf(2, "value = %s\n", ms->cur_tok->value);
	char	*cmd;
	t_list	*new_arg;

	cmd = NULL;
	new_arg = NULL;
	if (!ms->cur_tok->value)// une commande peut avoir comme valeur NULL dans ce cas: """"""
		cmd = NULL;
		// cmd = ft_calloc(1, sizeof(char));
	else
	{
		cmd = ft_strdup(ms->cur_tok->value);
		if (!cmd)
		{
			g_exit_code = 134;
			return (NULL);
		}
	}
	new_arg = ft_lstnew(cmd);
	if (!new_arg)
	{
		g_exit_code = 134;
		return (free(cmd), NULL);
	}
	// new_arg->n = ms->cur_tok->dol;
	ft_lstadd_back(&new_ast->args, new_arg);
	if (ms->cur_tok->dol)
		ft_doladd_back(&new_ast->dol, ms->cur_tok->dol);
	eat_token(ms, T_WORD);
	return (new_ast->args);
}

/*
 * handle_cmd
 * Si le token actuel est une parenthese => redirige vers la fonction handle_par
 * Sinon cree une structure AST de type commande
 * On cree les arguments de cet AST a travers une boucle qui continue tant que les tokens sont des words:
 * 	si word = redirection, le token suivant = le nom du fichier de redirection est ajoute en argument de la liste t_redirs
 * 	si word simple, on ajoute ce word a la t_list des arguments de la commande
 */

// t_ast	*handle_cmd(t_ms *ms)
// {
// 	t_ast	*new_ast;

// 	if (ms->cur_tok->type == T_LPAR)
// 		return (handle_par(ms));
// 	new_ast = new_node(ms, token_to_node(ms->cur_tok->type));
// 	if (!new_ast)
// 		return (NULL);
// 	while (ms->cur_tok && ms->cur_tok->type != T_PIPE
// 		&& ms->cur_tok->type != T_AND_IF && ms->cur_tok->type != T_OR_IF
// 		&& ms->cur_tok->type != T_EOF && ms->cur_tok->type != T_RPAR)
// 	{
// 		if (is_redir(ms->cur_tok->type))
// 		{
// 			if (!handle_red(ms, new_ast))
// 				return (free_root_ast(new_ast), NULL);
// 		}
// 		else
// 		{
// 			if (!add_cmd_args(ms, new_ast))
// 				return (free_root_ast(new_ast), NULL);
// 		}
// 	}
// 	return (new_ast);
// }
