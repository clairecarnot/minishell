/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:20:17 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/02 17:30:54 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

t_parser	*init_parser(t_lexer *lexer)
{
	t_parser	*parser;

	parser = ft_calloc(1, sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->lexer = lexer;
	parser->root = NULL;
	parser->cur_token = lexer->token_lst;
	return (parser);
}

void	parser(t_ms *minishell, t_lexer *lexer)
{
	minishell->parser = init_parser(lexer);
	if (!minishell->parser)
		free_minishell(minishell, 1);
	parse_grammar(minishell, minishell->parser);
}
