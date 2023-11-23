/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:34:23 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/22 11:09:13 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

t_token	*lexer_next_token_2(t_ms *minishell, t_lexer *lexer)
{
	if (lexer->src[lexer->cur_pos] == '(')
		return (advance(lexer), init_token(minishell, "(", T_LPAR));
	else if (lexer->src[lexer->cur_pos] == ')')
		return (advance(lexer), init_token(minishell, ")", T_RPAR));
	else if (lexer->src[lexer->cur_pos] == '<')
		return (advance(lexer), init_token(minishell, "<", T_LESS));
	else if (lexer->src[lexer->cur_pos] == '>')
		return (advance(lexer), init_token(minishell, ">", T_GREAT));
	// else if (lexer->src[lexer->cur_pos] == '\'' && \
	// lexer->src[lexer->cur_pos + 1] && lexer->src[lexer->cur_pos + 1] == '\'')
	// 	return (parse_following_quotes(minishell, lexer, 39));
	// else if (lexer->src[lexer->cur_pos] == '\"' && \
	// lexer->src[lexer->cur_pos + 1] && lexer->src[lexer->cur_pos + 1] == '\"')
	// 	return (parse_following_quotes(minishell, lexer, 34));
	else if (lexer->src[lexer->cur_pos] == '\'')
		return (parse_quotes_word(minishell, lexer, 39));
	else if (lexer->src[lexer->cur_pos] == '\"')
		return (parse_quotes_word(minishell, lexer, 34));
	else if (ft_ischar(lexer->src[lexer->cur_pos], 0))
		return (parse_word(minishell, lexer, 0));
	return (NULL);
}

t_token	*lexer_next_token(t_ms *minishell, t_lexer *lexer)
{
	while (is_wspace(lexer->src[lexer->cur_pos]) != 0)
		advance(lexer);
	if (lexer->src[lexer->cur_pos] == '|' && peek_next(lexer) == '|')
		return (advance_ntimes(lexer, 2), init_token(minishell, "||", T_OR_IF));
	else if (lexer->src[lexer->cur_pos] == '&' && peek_next(lexer) == '&')
		return (advance_ntimes(lexer, 2), \
		init_token(minishell, "&&", T_AND_IF));
	else if (lexer->src[lexer->cur_pos] == '<' && peek_next(lexer) == '<')
		return (advance_ntimes(lexer, 2), init_token(minishell, "<<", T_DLESS));
	else if (lexer->src[lexer->cur_pos] == '>' && peek_next(lexer) == '>')
		return (advance_ntimes(lexer, 2), \
		init_token(minishell, ">>", T_DGREAT));
	else if (lexer->src[lexer->cur_pos] == '\n')//a verifier car ne marche pas
		return (advance(lexer), init_token(minishell, "\n", T_NEWLINE));
	else if (lexer->src[lexer->cur_pos] == '|')
		return (advance(lexer), init_token(minishell, "|", T_PIPE));
	else
		return (lexer_next_token_2(minishell, lexer));
}

void	token_add_back(t_token **lst, t_token *new)
{
	t_token	*ptr;

	if (!new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		ptr = *lst;
		while (ptr->next_token)
			ptr = ptr->next_token;
		ptr->next_token = new;
	}
}

int	lexer(t_ms *minishell, char *s)
{
	minishell->lexer = init_lexer(s);
	if (!minishell->lexer)
		free_minishell(minishell, 1);
	while (s[minishell->lexer->cur_pos])
	{
		minishell->lexer->cur_c = s[minishell->lexer->cur_pos];
		token_add_back(&minishell->lexer->token_lst, \
		lexer_next_token(minishell, minishell->lexer));
	}
	token_add_back(&minishell->lexer->token_lst, \
	init_token(minishell, "\0", T_EOF));
	if (error_in_lexer(minishell) != 0)
		return (1);
	return (0);
}
