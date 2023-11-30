/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:52:28 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/29 17:41:14 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

/*
 * Convertit un type de token en un type d'ast
 */

int	token_to_node(int type)// a garder
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
 * "Mange" un token du type indique => fait avancer une liste de token au token suivant
 */

int	eat_token(t_ms *ms, t_type type)// a garder
{
	if (ms->cur_tok->type == type)
	{
		// dprintf(2, "eat\n");
		ms->cur_tok = ms->cur_tok->next_token;
		return (1);
	}
	return (0);
}
