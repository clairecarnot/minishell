/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:17:43 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:17:45 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "general.h"

//----------------------- lexer.c ------------------------
int			first_quote(t_lexer *lexer);
t_token		*lexer_next_token_2(t_ms *minishell, t_lexer *lexer);
t_token		*lexer_next_token(t_ms *minishell, t_lexer *lexer);
void		token_add_back(t_token **lst, t_token *new);
int			lexer(t_ms *minishell, char *s);

//--------------------- lexer_utils.c ----------------------
void		advance(t_lexer *lexer);
void		advntimes(t_lexer *lexer, int i);
int			is_wspace(char c);
char		peek_next(t_lexer *lexer);
int			ft_ischar(int c, int quotes);

//--------------------- lexer_utils2.c ----------------------
void		free_quote_state(t_ms *ms, t_dw *dw);
int			quote_size(t_ms *ms);
int			ft_isand(char *src, int cur_posi);
size_t		ft_strlen_lex(const char *s);
char		*quotes_strjoin(char *s1, char *s2, int size);

//--------------------- lexer_parse_word.c ----------------------
void		quote_state_close(t_ms *ms, int i, t_dw *dw);
void		quote_state_open(t_ms *ms, int qtype, int i, t_dw *dw);
int			case_value_null(t_ms *ms, t_dw *dw);
int			big_while(t_ms *ms);
t_token		*parse_quotes_word(t_ms *ms, int qtype);

//--------------------- lexer_parse_word2.c ----------------------
int			qstate(int i);
int			else_qtype(int qtype);
int			switch_qtype(t_ms *ms, int i, int qtype);
int			count_dol_chars_in(t_ms *ms, int i);
int			count_dol_chars_out(t_ms *ms, int i);

//--------------------- lexer_parse_word_wil.c ----------------------
int			update_lstwil(t_wil **wil, t_list *w);
void		update_lstwil_in(t_ms *ms, int qtype, int i, t_dw *dw);
int			update_lstwil_out(t_ms *ms, int i, t_dw *dw);
t_dw		*init_dw(t_ms *ms);

//--------------------- lexer_parse_word_dol.c ----------------------
int			update_lstdol(t_dol **dol, t_list *new_d, t_list *new_c);
void		update_lstdol_in(t_ms *ms, int qtype, int i, t_dw *dw);
int			update_lstdol_out(t_ms *ms, int i, int j, t_dw *dw);
int			case_w_dol(t_ms *ms, int qtype, t_dw *dw);

//--------------------- lexer_init.c ----------------------
t_lexer		*init_lexer(char *s);
t_token		*init_token(t_ms *minishell, char *value, t_type type);
t_token		*init_tokwdol(t_ms *minishell, t_type type, t_dw *dw);

//--------------------- lexer_error.c ----------------------
int			check_par(t_ms *ms);
int			alone_and_after_op(t_ms *ms);
int			consecutive_op(t_ms *ms);
int			error_in_lexer(t_ms *ms);

//--------------------- lexer_error_redir.c ----------------------
int			check_redir3(t_token *tok_lst);
int			check_redir2(t_token *tok_lst);
int			check_redir1(t_token *tok_lst);
int			check_redir(t_ms *ms);

//--------------------- prelexer_error.c ----------------------
int			check_after_lessgreat(char *line, size_t i);
int			check_redir_beforelex(char *line);
int			check_quotes(char *str);
int			check_error_prelexer(t_ms *ms);

#endif
