/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:03:23 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/16 16:42:28 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "parser.h"
# include "struct.h"
# include "export.h"

//----------------------- parser.c -----------------------
void	parse(t_ms *ms);
void	add_subtree(t_ast *node, t_ast	**root);
t_ast	*new_node(t_node_type type);

//----------------------- handle_cmds.c ------------------
t_ast	*handle_cmd(t_ms *ms);
t_list	*add_cmd_args(t_ms *ms, t_ast *new_ast);

//----------------------- handle_ops.c -------------------
t_ast	*handle_op(t_ms *ms);

//----------------------- handle_par.c -------------------
t_ast	*handle_par(t_ms *ms);

//----------------------- handle_pipes.c -----------------
t_ast	*handle_pipe(t_ms *ms);

//----------------------- handle_redirs.c ----------------
int	is_redir(int type);
t_redirs	*handle_red(t_ms *ms, t_ast *new_ast);
void	redirs_add_back(t_redirs **lst, t_redirs *new);
t_redirs	*redirs_new(t_token *token, int type);

//----------------------- utils.c ------------------------
int	token_to_node(int type);
int	eat_token(t_ms *ms, t_type type);


#endif