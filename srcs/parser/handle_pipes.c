/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:20:17 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/24 14:02:13 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

/*
 * handle_pipe:
 * Cree une structure AST de type PIPE
 * Le cas echeant, un autre AST qui deviendra la branche droite du pipe, est cree a travers une boucle (tant qu'on ne rencontre pas ')' ou '|')
 * On renvoie cet AST pipe qui sera ajoute "au-dessus" des noeuds precedants (a partir de sa branche gauche)
 */

// t_ast	*handle_pipe(t_ms *ms)
// {
// 	t_ast	*new_ast;
// 	t_ast	*tmp_tree;

// 	tmp_tree = NULL;
// 	new_ast = new_node(ms, token_to_node(T_PIPE));
// 	if (!new_ast)
// 		return (NULL);
// 	eat_token(ms, T_PIPE);
// 	while (ms->cur_tok && ms->cur_tok->type != T_PIPE
// 		&& ms->cur_tok->type != T_RPAR
// 		&& ms->cur_tok->type != T_EOF)
// 	{
// 		if (ms->cur_tok->type == T_AND_IF || ms->cur_tok->type == T_OR_IF)
// 			tmp_tree = handle_op(ms);
// 		else
// 			tmp_tree = handle_cmd(ms);
// 		if (!tmp_tree)
// 			return (free_root_ast(new_ast), NULL);
// 		add_subtree(tmp_tree, &new_ast->right);
// 	}
// 	return (new_ast);
// }
