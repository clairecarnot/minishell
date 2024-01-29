/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:34:23 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/29 14:10:42 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

/*
Renseigne sur le type de quote rencontree en 1er dans le mot
*/
int	first_quote(t_lexer *lexer)
{
	int	i;

	i = 0;
	while ((lexer->src[lexer->cur_pos + i] && \
	lexer->src[lexer->cur_pos + i] != ' '))
	{
		if (lexer->src[lexer->cur_pos + i] == '\"')
			return (34);
		if (lexer->src[lexer->cur_pos + i] == '\'')
			return (39);
		i++;
	}
	return (0);
}

t_token	*lexer_next_token_2(t_ms *minishell, t_lexer *lexer)
{
	int	qtype;

	if (lexer->src[lexer->cur_pos] == '(')
		return (advance(lexer), init_token(minishell, "(", T_LPAR));
	else if (lexer->src[lexer->cur_pos] == ')')
		return (advance(lexer), init_token(minishell, ")", T_RPAR));
	else if (lexer->src[lexer->cur_pos] == '<')
		return (advance(lexer), init_token(minishell, "<", T_LESS));
	else if (lexer->src[lexer->cur_pos] == '>')
		return (advance(lexer), init_token(minishell, ">", T_GREAT));
	else if (lexer->src[lexer->cur_pos] == '\'')
		return (parse_quotes_word(minishell, 39));
	else if (lexer->src[lexer->cur_pos] == '\"')
		return (parse_quotes_word(minishell, 34));
	else if (ft_ischar(lexer->src[lexer->cur_pos], 0))
	{
		qtype = first_quote(minishell->lexer);
		minishell->nb_q = 2;
		return (parse_quotes_word(minishell, qtype));
	}
	return (NULL);
}

t_token	*lexer_next_token(t_ms *minishell, t_lexer *lexer)
{
	minishell->i = 0;
	minishell->j = 0;
	minishell->nb_q = 0;
	while (is_wspace(lexer->src[lexer->cur_pos]) != 0)
		advance(lexer);
	if (lexer->src[lexer->cur_pos] == '|' && peek_next(lexer) == '|')
		return (advntimes(lexer, 2), init_token(minishell, "||", T_OR_IF));
	else if (lexer->src[lexer->cur_pos] == '&' && peek_next(lexer) == '&')
		return (advntimes(lexer, 2), \
		init_token(minishell, "&&", T_AND_IF));
	else if (lexer->src[lexer->cur_pos] == '<' && peek_next(lexer) == '<')
		return (advntimes(lexer, 2), init_token(minishell, "<<", T_DLESS));
	else if (lexer->src[lexer->cur_pos] == '>' && peek_next(lexer) == '>')
		return (advntimes(lexer, 2), \
		init_token(minishell, ">>", T_DGREAT));
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
		free_minishell(minishell, 255);
	minishell->cur_tok = NULL;
	while (s[minishell->lexer->cur_pos])
	{
		minishell->lexer->cur_c = s[minishell->lexer->cur_pos];
		token_add_back(&minishell->lexer->token_lst, \
		lexer_next_token(minishell, minishell->lexer));
		minishell->cur_tok = minishell->lexer->token_lst;
	}
	token_add_back(&minishell->lexer->token_lst, \
	init_token(minishell, "\0", T_EOF));
	if (error_in_lexer(minishell) != 0)
		return (1);
	return (0);
}
