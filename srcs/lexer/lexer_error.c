/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 10:46:00 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/15 14:15:27 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

int	check_redir3(t_token *tok_lst)
{
	if ((tok_lst->type == T_LESS || tok_lst->type == T_GREAT ||
	tok_lst->type == T_DGREAT || tok_lst->type == T_DLESS)
	&& tok_lst->next_token->type == T_EOF)// case : ...<eof or ...>eof or ...<<eof or ...>>eof		
		return (printf("minishell: syntax error near unexpected token `newline'\n"), 1);
	else if (tok_lst->type == T_DLESS && tok_lst->next_token->type == T_DLESS && 
	(tok_lst->next_token->next_token->type && tok_lst->next_token->next_token->type == T_DLESS))// case t_word <<<<<< t_word
		return (printf("minishell: syntax error near unexpected token `<<<'\n"), 1);
	else if (tok_lst->type == T_DLESS && tok_lst->next_token->type == T_DLESS && 
	(tok_lst->next_token->next_token->type && tok_lst->next_token->next_token->type == T_LESS))// case t_word <<<<< t_word
		return (printf("minishell: syntax error near unexpected token `<<'\n"), 1);
	else if (tok_lst->type == T_DLESS && tok_lst->next_token->type == T_DLESS)// case : t_word <<<< t_word
		return (printf("minishell: syntax error near unexpected token `<'\n"), 1);
	else if (tok_lst->type == T_DGREAT && tok_lst->next_token->type == T_DLESS)
		return (printf("minishell: syntax error near unexpected token `<<'\n"));// case : >> << 
	return (0);	
}

int	check_redir2(t_token *tok_lst)
{
	if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT &&
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_LESS)// case : <><
		return (printf("minishell: syntax error near unexpected token `<'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT &&
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_DLESS
	&& tok_lst->next_token->next_token->next_token && tok_lst->next_token->next_token->next_token->type == T_LESS)// case : <><<<<
		return (printf("minishell: syntax error near unexpected token `<<<'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT &&
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_DLESS
	&& tok_lst->next_token->next_token->next_token && tok_lst->next_token->next_token->next_token->type == T_DLESS)// case : <><<<<
		return (printf("minishell: syntax error near unexpected token `<<<'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT &&
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_DLESS)// case : <><<
		return (printf("minishell: syntax error near unexpected token `<<'\n"), 1);
	else if (tok_lst->type == T_GREAT && tok_lst->next_token->type == T_LESS)// case : >< 
		return (printf("minishell: syntax error near unexpected token `<'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT)// case : <     >
		return (printf("minishell: syntax error near unexpected token `>'\n"), 1);
	return (0);
}

int	check_redir1(t_token *tok_lst)
{
	if (tok_lst->type == T_DGREAT && tok_lst->next_token->type == T_DGREAT)// case t_word >>>> t_word
		return (printf("minishell: syntax error near unexpected token `>>'\n"), 1);
	else if (tok_lst->type == T_GREAT && tok_lst->next_token->type == T_GREAT)// case >  >
		return (printf("minishell: synthax error near unexpected token `>'\n"));
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_DGREAT && 
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_GREAT)// case : <>>>
		return (printf("minishell: syntax error near unexpected token `>>'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_DGREAT && 
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_DGREAT)// case : <>>>>
		return (printf("minishell: syntax error near unexpected token `>>'\n"), 1);
	else if (tok_lst->type == T_DGREAT && tok_lst->next_token->type == T_GREAT)// case : >>>eof or cmd >>> cmd		
		return (printf("minishell: syntax error near unexpected token `>'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_DGREAT)// case : <>>
		return (printf("minishell: syntax error near unexpected token `>'\n"), 1);
	else if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT &&
	tok_lst->next_token->next_token && tok_lst->next_token->next_token->type == T_LESS &&
	tok_lst->next_token->next_token->next_token && tok_lst->next_token->next_token->next_token->type == T_GREAT)// case : <  > <  >
		return (printf("minishell: syntax error near unexpected token `>'\n"), 1);
	return (0);
}

int	check_redir(t_ms *ms)
{
	t_token	*tok_lst = ms->lexer->token_lst;

	while (tok_lst && tok_lst->next_token)
	{
		if (check_redir1(tok_lst))
			return (1);
		else if (check_redir2(tok_lst))
			return (1);
		else if (check_redir3(tok_lst))
			return (1);
		tok_lst = tok_lst->next_token;
	}
	return (0);
}

int	check_par(t_ms *ms)
{
	int	i;
	int	lpar;
	int	rpar;
	t_token *tok_lst = ms->lexer->token_lst;

	i = 0;
	lpar = 0;
	rpar = 0;
	while (tok_lst && tok_lst->next_token)
	{
		if (tok_lst->type == T_LPAR)
			lpar++;
		else if (tok_lst->type == T_RPAR)
			rpar++;
		tok_lst = tok_lst->next_token;
	}
	if (lpar != rpar)
		return (printf("parenthesis error\n"), 1);
	return (0);
}

int	alone_and_after_op(t_ms *ms)
{
	t_token	*tok_lst = ms->lexer->token_lst;

	while (tok_lst && tok_lst->next_token)
	{
		if ((tok_lst->type == T_PIPE || tok_lst->type == T_AND_IF 
		|| tok_lst->type == T_OR_IF) && tok_lst->next_token->type == T_WORD)
		{
			if (!ft_strncmp(tok_lst->next_token->value, "&", 1))// if next_token->value == '&'
				return (printf("minishell: syntax error near unexpected token `&'\n"), 1);
		}
		tok_lst = tok_lst->next_token;
	}
	return (0);
}

int	consecutive_op(t_ms *ms)
{
	t_token *tok_lst = ms->lexer->token_lst;

	while (tok_lst && tok_lst->next_token)
	{
		if (tok_lst->type == T_PIPE && (tok_lst->next_token->type == T_PIPE 
		|| tok_lst->next_token->type == T_AND_IF || tok_lst->next_token->type == T_OR_IF))
			return (printf("minishell: syntax error near unexpected token `%s'\n", tok_lst->next_token->value), 1);
		else if (tok_lst->type == T_AND_IF && (tok_lst->next_token->type == T_PIPE 
		|| tok_lst->next_token->type == T_AND_IF || tok_lst->next_token->type == T_OR_IF))
			return (printf("minishell: syntax error near unexpected token `%s'\n", tok_lst->next_token->value), 1);
		else if (tok_lst->type == T_OR_IF && (tok_lst->next_token->type == T_PIPE 
		|| tok_lst->next_token->type == T_AND_IF || tok_lst->next_token->type == T_OR_IF))
			return (printf("minishell: syntax error near unexpected token `%s'\n", tok_lst->next_token->value), 1);
		tok_lst = tok_lst->next_token;
	}
	return (0);
}

int	error_in_lexer(t_ms *ms)//check les erreurs detectables au niveau lexer, return 1 if error, 0 if success
{
	t_token *tok_lst = ms->lexer->token_lst;
	
	//the first token is an operator
	if (tok_lst->type == T_PIPE || tok_lst->type == T_AND_IF || tok_lst->type == T_OR_IF || tok_lst->type == T_RPAR)
		return (printf("minishell: syntax error near unexpected token `%s'\n", ms->lexer->token_lst->value), 1);

	//parenthesis pb
	if (check_par(ms) != 0)
		return (1);

	//two or more operator comes after another
	if (consecutive_op(ms) != 0)
		return (1);
	if (alone_and_after_op(ms) != 0)
		return (1);
	
	if (check_redir(ms) != 0)
		return (1);

	return(0);
}
