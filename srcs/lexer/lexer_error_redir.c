/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:20:15 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/30 15:43:33 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

// case t_word <<<<<< t_word
// case t_word <<<<< t_word
// case : t_word <<<< t_word
// case : >> <<
// all other cases ...
int	check_redir4(t_token *tok_lst)
{	
	if (tok_lst->type == T_DLESS && tok_lst->next_token \
	->type == T_DLESS && (tok_lst->next_token->next_token->type \
	&& tok_lst->next_token->next_token->type == T_DLESS))
		return (printf("0minishell: \
syntax error near unexpected token `<<<'\n"), 1);
	else if (tok_lst->type == T_DLESS && tok_lst->next_token \
	->type == T_DLESS && (tok_lst->next_token->next_token->type \
	&& tok_lst->next_token->next_token->type == T_LESS))
		return (printf("00minishell: \
syntax error near unexpected token `<<'\n"), 1);
	else if (tok_lst->type == T_DLESS && tok_lst->next_token \
	->type == T_DLESS)
		return (printf("000minishell: \
syntax error near unexpected token `<'\n"), 1);
	else if (tok_lst->type == T_DGREAT && tok_lst->next_token \
	->type == T_DLESS)
		return (printf("0000minishell: \
syntax error near unexpected token `<<'\n"));
	else if ((tok_lst->type == T_LESS || tok_lst->type == T_GREAT \
	|| tok_lst->type == T_DLESS || tok_lst->type == T_DGREAT) \
	&& (tok_lst->next_token->type != T_WORD))
		return (printf("00000minishell: syntax error near unexpected token\n"), 1);
	return (0);
}

// case : <><<<<
// case : <><<
// case : >< 
// case : <     >
// case : ...<eof or ...>eof or ...<<eof or ...>>eof
int	check_redir3(t_token *tok_lst)
{	
	if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT && \
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type \
	== T_DLESS && tok_lst->next_token->next_token->next_token && \
	tok_lst->next_token->next_token->next_token->type == T_DLESS)
		return (printf("1minishell: \
syntax error near unexpected token `<<<'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == \
	T_GREAT && tok_lst->next_token->next_token && tok_lst->next_token \
	->next_token->type == T_DLESS)
		return (printf("11minishell: \
syntax error near unexpected token `<<'\n"), 1);
	else if (tok_lst->type == T_GREAT && tok_lst->next_token->type == T_LESS)
		return (printf("111minishell: \
syntax error near unexpected token `<'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT)
		return (printf("1111minishell: \
syntax error near unexpected token `>'\n"), 1);
	if ((tok_lst->type == T_LESS || tok_lst->type == T_GREAT || \
	tok_lst->type == T_DGREAT || tok_lst->type == T_DLESS) \
	&& tok_lst->next_token->type == T_EOF)
		return (printf("11111minishell: \
syntax error near unexpected token `newline'\n"), 1);
	return (0);
}

// case : <  > <  >
// case : <><
// case : <><<<<
int	check_redir2(t_token *tok_lst)
{	
	if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT && \
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type \
	== T_LESS && tok_lst->next_token->next_token->next_token && \
	tok_lst->next_token->next_token->next_token->type == T_GREAT)
		return (printf("2minishell: \
syntax error near unexpected token `>'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == \
	T_GREAT && tok_lst->next_token->next_token && tok_lst-> \
	next_token->next_token->type == T_LESS)
		return (printf("22minishell: \
syntax error near unexpected token `<'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type \
	== T_GREAT && tok_lst->next_token->next_token && tok_lst-> \
	next_token->next_token->type == T_DLESS && tok_lst->next_token \
	->next_token->next_token && tok_lst->next_token->next_token-> \
	next_token->type == T_LESS)
		return (printf("222minishell: \
syntax error near unexpected token `<<<'\n"), 1);
	return (0);
}

// case t_word >>>> t_word
// case >  >
// case : <>>>
// case : <>>>>
// case : >>>eof or cmd >>> cmd
// case : <>>
int	check_redir1(t_token *tok_lst)
{
	if (tok_lst->type == T_DGREAT && tok_lst->next_token->type == T_DGREAT)
		return (printf("3minishell: \
syntax error near unexpected token `>>'\n"), 1);
	else if (tok_lst->type == T_GREAT && tok_lst->next_token->type == T_GREAT)
		return (printf("33minishell: \
synthax error near unexpected token `>'\n"));
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == \
	T_DGREAT && tok_lst->next_token->next_token && tok_lst->next_token \
	->next_token->type == T_GREAT)
		return (printf("333minishell: \
syntax error near unexpected token `>>'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == \
	T_DGREAT && tok_lst->next_token->next_token && tok_lst->next_token \
	->next_token->type == T_DGREAT)
		return (printf("3333minishell: \
syntax error near unexpected token `>>'\n"), 1);
	else if (tok_lst->type == T_DGREAT && tok_lst->next_token->type == T_GREAT)
		return (printf("33333minishell: \
syntax error near unexpected token `>'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_DGREAT)
		return (printf("333333minishell: \
syntax error near unexpected token `>'\n"), 1);
	return (0);
}

int	check_redir(t_ms *ms)
{
	t_token	*tok_lst;

	tok_lst = ms->lexer->token_lst;
	while (tok_lst && tok_lst->next_token)
	{
		if (check_redir1(tok_lst))
			return (1);
		else if (check_redir2(tok_lst))
			return (1);
		else if (check_redir3(tok_lst))
			return (1);
		else if (check_redir4(tok_lst))
			return (1);
		tok_lst = tok_lst->next_token;
	}
	return (0);
}
