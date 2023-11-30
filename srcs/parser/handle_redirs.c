/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:20:17 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/13 15:10:43 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

/*
 * redirs_add_back
 * Ajoute un element t_redirs a la fin d'une liste t_redirs
 */

void	redirs_add_back(t_redirs **lst, t_redirs *new)
{
	t_redirs	*ptr;

	if (*lst == NULL)
		*lst = new;
	else
	{
		ptr = *lst;
		while (ptr->next_redir)
			ptr = ptr->next_redir;
		ptr->next_redir = new;
	}
}

/*
 * redirs_new:
 * Cree une nouvelle structure t_redirs du type indique par le current token
 * Le nom du fichier vers lequel la redir se fait est indique par le next token
 * La next redir est initialisee a NULL
 */

t_redirs	*redirs_new(t_token *token, int type)
{
	t_redirs	*d;

	d = malloc(sizeof(t_redirs));
	if (!d)
		return (NULL);
	d->type = token_to_node(type);
	d->filename = ft_strdup(token->value);
	if (!d->filename)
		return (free(d), NULL);
	d->next_redir = NULL;
	return (d);
}

/*
 * is_redir:
 * Verifie que le type envoye est un token redirection
 */

// int	is_redir(int type)
// {
// 	if (type == T_LESS || type == T_GREAT
// 		|| type == T_DLESS || type == T_DGREAT)
// 		return (1);
// 	return (0);
// }

/*
 * handle_red:
 * Cree une nouvelle structure t_redirs qui contient le nom du fichier correspondant a cette redirection. Cette structure est ajoutee a la liste des redirections de l'AST
 */

t_redirs	*handle_red(t_ms *ms, t_ast *new_ast)
{
	t_redirs	*new_redir;

	if (ms->cur_tok->next_token->type != T_WORD)
		return (NULL);
	new_redir = redirs_new(ms->cur_tok->next_token, ms->cur_tok->type);
	if (!new_redir)
		return (NULL);
	redirs_add_back(&new_ast->redirs, new_redir);
	eat_token(ms, ms->cur_tok->type);
	eat_token(ms, ms->cur_tok->type);
	return (new_ast->redirs);
}
