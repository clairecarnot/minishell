/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse_word.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:30:04 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/15 14:36:53 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

t_token	*parse_quotes_word(t_ms *ms, t_lexer *lexer, int qtype)//qtype = quote type
{
	char	*value;
	int		i;

	i = 1;
	while (lexer->src[lexer->cur_pos + i] && ft_ischar(lexer->src[lexer->cur_pos + i], 1) && lexer->src[lexer->cur_pos + i] != qtype)
		i++;
	value =  ft_calloc(i + 2, sizeof(char));
	if (!value)//verifier protec
		return (NULL);
	ft_strlcpy(value, &(lexer->src[lexer->cur_pos]), i + 2);
	advance_ntimes(lexer, i + 1);
	return (init_token(ms, value, T_WORD));
}

t_token	*parse_word(t_ms *minishell, t_lexer *lexer)
{
	char	*value;
	int		i;

	i = 0;
	while (lexer->src[lexer->cur_pos + i] && ft_ischar(lexer->src[lexer->cur_pos + i], 0))
		i++;
	value = ft_calloc(i + 1, sizeof(char));
	if (!value)
		return (NULL);
	ft_strlcpy(value, &(lexer->src[lexer->cur_pos]), i + 1);
	advance_ntimes(lexer, i);
	return (init_token(minishell, value, T_WORD));
}
