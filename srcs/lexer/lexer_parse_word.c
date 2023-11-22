/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse_word.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:30:04 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/21 17:44:57 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

t_token	*parse_following_quotes(t_ms *ms, t_lexer *lexer, int qtype)
{	
	int	i;
	int	j;
	char	*value;

	i = 0;
	j = 0;
	while (lexer->src[lexer->cur_pos + i] && lexer->src[lexer->cur_pos + i] == qtype)
		i++;
	if (!lexer->src[lexer->cur_pos + i] || \
	(lexer->src[lexer->cur_pos + i] && lexer->src[lexer->cur_pos + i] == ' ' && i % 2 == 0))//cas """"  hey  """" -> T_WORD1="" T_WORD2="hey" T_WORD3=""
	{
		value = ft_calloc(1, sizeof(char));
		value = NULL;
		advance_ntimes(lexer, i);
		return (init_token(ms, value, T_WORD));
	}
	return (parse_quotes_word(ms, lexer, qtype));
}

//qtype = quote type
t_token	*parse_quotes_word(t_ms *ms, t_lexer *lexer, int qtype)
{
	int		i;
	int		q_nb;
	char	*value;

	i = 0;
	q_nb = 0;
	while (lexer->src[lexer->cur_pos + i] && lexer->src[lexer->cur_pos + i] == qtype)
	{
		i++;
		q_nb++;
	}
	while (lexer->src[lexer->cur_pos + i] && \
	ft_ischar(lexer->src[lexer->cur_pos + i], 1) && \
	lexer->src[lexer->cur_pos + i] != qtype)
		i++;
	value = ft_calloc(i, sizeof(char));
	if (!value)//verifier protec
		return (NULL);
	ft_strlcpy(value, &(lexer->src[lexer->cur_pos + q_nb]), i - q_nb + 1);//peut-etre a proteger
	while(lexer->src[lexer->cur_pos + i] && \
	(lexer->src[lexer->cur_pos + i] == '\'' || lexer->src[lexer->cur_pos + i] == '\"'))
		i++;
	advance_ntimes(lexer, i);
	return (init_token(ms, value, T_WORD));
}

t_token	*parse_word(t_ms *minishell, t_lexer *lexer, int i)
{
	char	*value;
	// int		i;

	// i = 0;
	while (lexer->src[lexer->cur_pos + i] && \
	ft_ischar(lexer->src[lexer->cur_pos + i], 0))
		i++;
	value = ft_calloc(i + 1, sizeof(char));
	if (!value)
		return (NULL);
	ft_strlcpy(value, &(lexer->src[lexer->cur_pos]), i + 1);//peut etre a proteger
	advance_ntimes(lexer, i);
	return (init_token(minishell, value, T_WORD));
}
