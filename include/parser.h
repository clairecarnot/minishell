/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:03:23 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/02 14:42:59 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "parser.h"
# include "struct.h"

//----------------------- parser.c ------------------------
t_parser	*init_parser(t_lexer *lexer);
void    parser(t_ms *minishell, t_lexer *lexer);

//----------------------- grammar.c ------------------------
t_ast	*new_node(t_ms *minishell, t_node_type type);
void	join_node(t_ast *root, t_ast *left, t_ast *right);
t_ast *read_words(t_ms *minishell, t_parser *parser);
t_ast	*pipeline(t_ms *minishell, t_parser *parser);
void    parse_grammar(t_ms *minishell, t_parser *parser);

//----------------------- utils.c ------------------------
int	eat_token(t_parser *parser, t_type type);

#endif