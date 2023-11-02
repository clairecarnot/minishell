/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:34:23 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/02 18:26:26 by mapoirie         ###   ########.fr       */
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

t_token	*init_token(t_ms *minishell, char *value, t_type type)
{
	t_token	*token;


	token = ft_calloc(1, sizeof(t_token));
	if (!token)
	{
		if (value)
			free(value);
		free_minishell(minishell, 1);
	}
	token->type = type;
	token->value = value;
	token->next_token = NULL;
	return (token);
}

void	advance(t_lexer *lexer, int i)
{
	lexer->cur_pos += i;
}

t_token	*parse_word(t_ms *minishell, t_lexer *lexer)
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
	return (init_token(minishell, value, T_WORD));
}

int	is_wspace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

t_token	*lexer_next_token(t_ms *minishell, t_lexer * lexer)
{
	while (is_wspace(lexer->src[lexer->cur_pos]) != 0)
		advance(lexer, 1);
	if (lexer->src[lexer->cur_pos] == '|')
		return (advance(lexer, 1), init_token(minishell, "|", T_PIPE));
	else if (ft_isalpha(lexer->src[lexer->cur_pos]))
		return (parse_word(minishell, lexer));
	return (NULL);
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

void	lexer(t_ms *minishell, char *s)
{

	minishell->lexer = init_lexer(s);
	if (!minishell->lexer)
		free_minishell(minishell, 1);
	while (s[minishell->lexer->cur_pos])
	{
		minishell->lexer->cur_c = s[minishell->lexer->cur_pos];
		token_add_back(&minishell->lexer->token_lst, lexer_next_token(minishell, minishell->lexer));
	}
	token_add_back(&minishell->lexer->token_lst, init_token(minishell, "\0", T_EOF));
}
