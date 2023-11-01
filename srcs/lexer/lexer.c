/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:34:23 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/01 15:41:13 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_lexer	*init_lexer(char *s)
{
	t_lexer	*lexer;

	if (!s)
		return (NULL);
	lexer = ft_calloc(1, sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->src = s;
	lexer->src_size = ft_strlen(s);
	lexer->cur_c = s[0];
	lexer->cur_pos = 0;
	lexer->token_lst = NULL;
	return (lexer);
}

t_token	*init_token(t_lexer *value, t_type type)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token);
		return (NULL);
	token->type = type;
	token->value = value;
	token->next_token = NULL;
	return (token);
}

void	advance(t_lexer *lexer, int i)
{
	lexer->cur_pos += i;
}

t_token	*parse_word(t_lexer *lexer)
{
	char	*value;
	int	i;

	i = 0;
	while (lexer->src[cur_pos + i] && ft_isalpha(lexer->src[cur_pos + i]))
		i++;
	value = ft_calloc(i + 1, sizeof(char));
	if (!value)
		return (NULL);
	ft_strlcpy(value, &(lexer->src[cur_pos]), i);
	advance(lexer, i);
	return (init_token(value, T_WORD));
}

t_token	*lexer_next_token(t_lexer * lexer)
{
	if (lexer->cur_c == '\0')
		return (init_token("\0", T_EOF));
	else if (ft_isalpha(lexer->cur_c))
		return (parse_word(lexer));
}

t_token	*lexer(char *s)
{
	t_lexer	lexer;

	lexer = init_lexer(s);
	if (!lexer)
		return (NULL);
	while (s[lexer->cur_pos])
	{
		lexer->cur_c = s[lexer->cur_pos];
		lexer->token_lst = lexer_next_token(lexer);
		if (!lexer->token_lst)
			return (NULL);
		lexer->token_lst = lexer->token_lst->next_token;
		lexer->cur_pos++; //WARNING
	}
	return (token_to_str(lexer->token_lst)); //?
}
