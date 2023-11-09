/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 10:46:00 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/09 17:27:30 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

int	alone_and_after_op(t_ms *ms)
{
	t_token	*tok_lst = ms->lexer->token_lst;

	while (tok_lst && tok_lst->next_token)
	{
		if ((tok_lst->type == T_PIPE || tok_lst->type == T_AND_IF 
		|| tok_lst->type == T_OR_IF) && tok_lst->next_token->type == T_WORD)
		{
			if (!ft_strncmp(tok_lst->next_token->value, "&", 1))// if next_token->value == '&'
				return (printf("minishell: syntax error near unexpected token `&'\n"), 0);
		}
		tok_lst = tok_lst->next_token;
	}
	return (1);
}

int	consecutive_op(t_ms *ms)
{
	t_token *tok_lst = ms->lexer->token_lst;

	while (tok_lst && tok_lst->next_token)
	{
		if (tok_lst->type == T_PIPE && (tok_lst->next_token->type == T_PIPE 
		|| tok_lst->next_token->type == T_AND_IF || tok_lst->next_token->type == T_OR_IF))
			return (printf("minishell: syntax error near unexpected token `%s'\n", tok_lst->next_token->value), 0);
		else if (tok_lst->type == T_AND_IF && (tok_lst->next_token->type == T_PIPE 
		|| tok_lst->next_token->type == T_AND_IF || tok_lst->next_token->type == T_OR_IF))
			return (printf("minishell: syntax error near unexpected token `%s'\n", tok_lst->next_token->value), 0);
		else if (tok_lst->type == T_OR_IF && (tok_lst->next_token->type == T_PIPE 
		|| tok_lst->next_token->type == T_AND_IF || tok_lst->next_token->type == T_OR_IF))
			return (printf("minishell: syntax error near unexpected token `%s'\n", tok_lst->next_token->value), 0);
		tok_lst = tok_lst->next_token;
	}
	return (1);
}

int	consecutive_redir(t_ms *ms)
{
	t_token	*tok_lst = ms->lexer->token_lst;

	if (ms->lexer->tok_count == 3)// case <>
	{	
		if (tok_lst->type == T_LESS && tok_lst->next_token->type == T_GREAT)
			return (printf("minishell: syntax error near unexpected token `newline'\n"), 0);
	}
	return (1);
}

int	error_in_lexer(t_ms *ms)//check les erreurs detectables au niveau lexer, return 0 if error, 1 if no error
{
	t_token *tok_lst = ms->lexer->token_lst;
	
	//the first token is an operator
	if (tok_lst->type == T_PIPE || tok_lst->type == T_AND_IF || tok_lst->type == T_OR_IF || tok_lst->type == T_RPAR)
		return (printf("minishell: syntax error near unexpected token `%s'\n", ms->lexer->token_lst->value), 0);
	
	//there is only 2 token, the first is a redirection
	if (ms->lexer->tok_count == 2)
	{	
		if (tok_lst->type == T_LESS || tok_lst->type == T_GREAT || tok_lst->type == T_DLESS || tok_lst->type == T_DGREAT)
			return (printf("minishell: syntax error near unexpected token `newline'\n"), 0);
	}

	

	//two or more operator comes after another
	if (!consecutive_op(ms))
		return (0);
	if (!alone_and_after_op(ms))
		return (0);
	if (!consecutive_redir(ms))
		return (0);
	return(1);
}

