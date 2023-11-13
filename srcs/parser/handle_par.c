/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_par.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:20:17 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/13 15:12:33 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

/*
 * handle_par:
 * Boucle tant qu'on n'a pas recupere une ')'
 * La boucle recupere une structure AST du type correspondant au token suivant la parenthese
 * La boucle ajoute chaque AST recupere au-dessus dans l'ancien AST, et renvoie l'AST final avec un flag subshell = 1
 */

t_ast	*handle_par(t_ms *ms)
{
	t_ast	*new_ast;
	t_ast	*tmp_tree;

	new_ast = NULL;
	eat_token(ms, T_LPAR);
	while (ms->cur_tok && ms->cur_tok->type != T_RPAR)
	{
		if (ms->cur_tok->type == T_PIPE)
			tmp_tree = handle_pipe(ms);
		else if (ms->cur_tok->type == T_AND_IF || ms->cur_tok->type == T_OR_IF)
			tmp_tree = handle_op(ms);
		else
			tmp_tree = handle_cmd(ms);
		if (!tmp_tree)
			return (free_root_ast(new_ast), NULL);
		if (!new_ast)
			new_ast = tmp_tree;
		else
			add_subtree(tmp_tree, &new_ast);
	}
	eat_token(ms, T_RPAR);
	new_ast->subsh = 1;
	return (new_ast);
}
