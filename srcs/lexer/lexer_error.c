/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 10:46:00 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/30 15:40:19 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

int	check_par(t_ms *ms)
{
	int		lpar;
	int		rpar;
	t_token	*tok_lst;

	lpar = 0;
	rpar = 0;
	tok_lst = ms->lexer->token_lst;
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
	t_token	*tok_lst;

	tok_lst = ms->lexer->token_lst;
	while (tok_lst && tok_lst->next_token)
	{
		if ((tok_lst->type == T_PIPE || tok_lst->type == T_AND_IF \
		|| tok_lst->type == T_OR_IF) && tok_lst->next_token->type == T_WORD)
		{
			if (!ft_strncmp(tok_lst->next_token->value, "&", 1))
				return (printf("minishell: \
syntax error near unexpected token `&'\n"), 1);
		}
		tok_lst = tok_lst->next_token;
	}
	return (0);
}

int	consecutive_op(t_ms *ms)
{
	t_token	*tok_lst;

	tok_lst = ms->lexer->token_lst;
	while (tok_lst && tok_lst->next_token)
	{
		if (tok_lst->type == T_PIPE && (tok_lst->next_token->type == T_PIPE || \
		tok_lst->next_token->type == T_AND_IF || tok_lst->next_token->type \
		== T_OR_IF))
			return (printf("minishell: syntax error near \
unexpected token `%s'\n", tok_lst->next_token->value), 1);
		else if (tok_lst->type == T_AND_IF && (tok_lst->next_token->type \
		== T_PIPE || tok_lst->next_token->type == T_AND_IF || tok_lst-> \
		next_token->type == T_OR_IF))
			return (printf("minishell: syntax error near \
unexpected token `%s'\n", tok_lst->next_token->value), 1);
		else if (tok_lst->type == T_OR_IF && (tok_lst->next_token->type \
		== T_PIPE || tok_lst->next_token->type == T_AND_IF || tok_lst-> \
		next_token->type == T_OR_IF))
			return (printf("minishell: syntax error near \
unexpected token `%s'\n", tok_lst->next_token->value), 1);
		tok_lst = tok_lst->next_token;
	}
	return (0);
}

//check les erreurs detectables au niveau lexer, return 1 if error, 0 if success
// line 94-95 the first token is an operator
// line 98 parenthesis pb
int	error_in_lexer(t_ms *ms)
{
	t_token	*tok_lst;

	tok_lst = ms->lexer->token_lst;
// 	if (tok_lst->type == T_PIPE || tok_lst->type == T_AND_IF || \
// 	tok_lst->type == T_OR_IF || tok_lst->type == T_RPAR)
// 		return (printf("minishell: syntax error near \
// unexpected token `%s'\n", ms->lexer->token_lst->value), 1);
	if (check_par(ms) != 0)
		return (1);
	// if (consecutive_op(ms) != 0)
	// 	return (1);
	// if (alone_and_after_op(ms) != 0)
	// 	return (1);
	// if (check_redir(ms) != 0)
	// 	return (1);
	return (0);
}
