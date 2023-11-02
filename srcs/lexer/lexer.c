/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:34:23 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/02 10:21:05 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

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

t_token	*init_token(char *value, t_type type)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
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
	while (lexer->src[lexer->cur_pos + i] && ft_isalpha(lexer->src[lexer->cur_pos + i]))
		i++;
	value = ft_calloc(i + 1, sizeof(char));
	if (!value)
		return (NULL);
	ft_strlcpy(value, &(lexer->src[lexer->cur_pos]), i + 1);
	advance(lexer, i);
	return (init_token(value, T_WORD));
}

int	is_wspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

t_token	*lexer_next_token(t_lexer * lexer)
{
	while (is_wspace(lexer->src[lexer->cur_pos]) != 0)
		advance(lexer, 1);
	if (ft_isalpha(lexer->src[lexer->cur_pos]))
		return (parse_word(lexer));
	return (NULL);
}

void	token_add_back(t_token **lst, t_token *new)
{
	t_token	*ptr;

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

t_lexer	*lexer(char *s)
{
	t_lexer	*lexer;
	t_token	*tmp_token;

	lexer = init_lexer(s);
	if (!lexer)
		return (NULL);
	while (s[lexer->cur_pos])
	{
		lexer->cur_c = s[lexer->cur_pos];
		token_add_back(&lexer->token_lst, lexer_next_token(lexer));
		if (!lexer->token_lst)
			return (NULL);
	}
	tmp_token = init_token("\0", T_EOF);
	if (!tmp_token)
		return(NULL);
	token_add_back(&lexer->token_lst, tmp_token);
	return (lexer);
}
