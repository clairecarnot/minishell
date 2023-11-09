/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:03:23 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/09 16:33:20 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "parser.h"
# include "struct.h"

//----------------------- parser.c ------------------------
void	parse(t_ms *ms);
void	add_subtree(t_ast *node, t_ast	**root);
t_ast	*handle_op(t_ms *ms);
t_ast	*handle_cmd(t_ms *ms);
t_ast	*handle_par(t_ms *ms);
t_ast	*handle_pipe(t_ms *ms);
int	is_redir(int type);
t_redirs	*handle_red(t_ms *ms, t_ast *new_ast);
t_ast	*new_node(t_node_type type);


//----------------------- grammar.c ------------------------
// t_ast	*new_node(t_ms *minishell, t_node_type type);
// void	join_node(t_ast *root, t_ast *left, t_ast *right);
// t_ast *read_words(t_ms *minishell, t_parser *parser);
// t_ast	*pipeline(t_ms *minishell, t_parser *parser);
// void    parse_grammar(t_ms *minishell, t_parser *parser);

//----------------------- utils.c ------------------------
int	token_to_node(int type);
int	eat_token(t_ms *ms, t_type type);
void	redirs_add_back(t_redirs **lst, t_redirs *new);
t_redirs	*redirs_new(t_token *token, int type);


#endif