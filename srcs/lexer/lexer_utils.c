/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 16:46:22 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/30 15:21:25 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

void	advance(t_lexer *lexer)
{
	if (lexer->next_pos >= lexer->src_size)
		lexer->cur_c = '\0';
	else
		lexer->cur_c = lexer->src[lexer->next_pos];
	lexer->cur_pos += 1;
	lexer->next_pos += 1;
}

void	advance_ntimes(t_lexer *lexer, int i)
{
	while (i > 0)
	{
		advance(lexer);
		i--;
	}
}

int	is_wspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

char	peek_next(t_lexer *lexer)
{
	if (lexer->next_pos >= lexer->src_size)
		return ('\0');
	return (lexer->src[lexer->next_pos]);
}

int	ft_ischar(int c, int quotes)
{
	if (!quotes)
	{
		if (c != ' ' && c != '(' && c != ')' \
		&& c != '<' && c != '>' && c != '|' && \
		c != '&' && c != 0) /*\
		&& c != 34 && c != 39)*/
			return (1);
	}
	else if (quotes == 1)
	{
		if (c != 0)
			return (1);
	}
	return (0);
}

/*
32 = SPACE
38 = &
40 = (
41 = )
60 = <
62 = >
124 = |
0 = '\0'
*/