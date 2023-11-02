/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:52:28 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/02 12:52:56 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

int	eat_token(t_parser *parser, t_type type)
{
	if (parser->cur_token->type == type)
	{
		parser->cur_token = parser->cur_token->next_token;
		return (1);
	}
	return (0);
}