/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:17:27 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:17:28 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "general.h"

//----------------------- parser.c -----------------------
t_ast		*factor(t_ms *ms);
t_ast		*handle_par(t_ms *ms);
t_ast		*term(t_ms *ms);
t_ast		*expr(t_ms *ms);
int			parse(t_ms *ms);

//----------------------- handle_cmds.c ------------------
t_dol		*dup_tokdol(t_ms *ms, t_dol *tok_dol);
int			ft_doladd_back(t_ms *ms, t_dol **dol, t_dol *new);
int			ft_wiladd_back(t_ms *ms, t_wil **wil, t_wil *new);
t_list		*add_cmd_args(t_ms *ms, t_ast *new_ast);

//----------------------- handle_redirs.c ----------------
void		redirs_add_back(t_redirs **lst, t_redirs *new);
t_redirs	*redirs_new(t_ms *ms, t_token *token, int type);
t_redirs	*handle_red(t_ms *ms, t_ast *new_ast);
int			is_redirect(t_ms *ms, t_token *cur_tok);

//----------------------- utils.c ------------------------
t_ast		*new_node(t_ms *ms, t_node_type type);
int			token_to_node(int type);
int			eat_token(t_ms *ms, t_type type);
t_list		*lst_dup_int(t_list *lst_tocpy);

#endif
