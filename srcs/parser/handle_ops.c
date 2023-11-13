/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:20:17 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/13 15:12:23 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

/*
 * handle_op:
 * Cree une structure AST de type operateur (AND ou OR)
 * AND/OR est normalement forcement suivi d'un token word (word, parenthese ou redirection): on cree donc une autre structure AST qui sera la branche droite de l'AST operateur
 * Il n'y a pas de boucle puisque l'operateur n'a qu'une seule commande a sa droite
 * L'AST operateur renvoye sera implante au-dessus de l'ancienne root, a partir de sa branche gauche
 */

t_ast	*handle_op(t_ms *ms)
{
	t_ast	*new_ast;
	t_ast	*tmp_tree;

	tmp_tree = NULL;
	new_ast = new_node(token_to_node(ms->cur_tok->type));
	if (!new_ast)
		return (NULL);
	eat_token(ms, ms->cur_tok->type);
	if (ms->cur_tok && ms->cur_tok->type != T_EOF
		&& (ms->cur_tok->type == T_WORD
			|| ms->cur_tok->type >= T_LPAR))
	{
		new_ast->right = handle_cmd(ms);
		if (!new_ast->right)
			return (free_root_ast(new_ast), NULL);
	}
	return (new_ast);
}
