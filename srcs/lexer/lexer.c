/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:34:23 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/09 14:12:00 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

t_token	*parse_word(t_ms *minishell, t_lexer *lexer)
{
	char	*value;
	int	i;

	i = 0;
	while (lexer->src[lexer->cur_pos + i] && ft_ischar(lexer->src[lexer->cur_pos + i]))
		i++;
	value = ft_calloc(i + 1, sizeof(char));
	if (!value)
		return (NULL);
	ft_strlcpy(value, &(lexer->src[lexer->cur_pos]), i + 1);
	advance_ntimes(lexer, i);
	return (init_token(minishell, value, T_WORD));
}


t_token	*lexer_next_token(t_ms *minishell, t_lexer * lexer)
{
	while (is_wspace(lexer->src[lexer->cur_pos]) != 0)
		advance(lexer);
	if (lexer->src[lexer->cur_pos] == '|' && peek_next(lexer) == '|')
		return (advance_ntimes(lexer, 2), init_token(minishell, "||", T_OR_IF));
	else if (lexer->src[lexer->cur_pos] == '&' && peek_next(lexer) == '&')
		return (advance_ntimes(lexer, 2), init_token(minishell, "&&", T_AND_IF));
	else if (lexer->src[lexer->cur_pos] == '<' && peek_next(lexer) == '<')
		return (advance_ntimes(lexer, 2), init_token(minishell, "<<", T_DLESS));
	else if (lexer->src[lexer->cur_pos] == '>' && peek_next(lexer) == '>')
		return (advance_ntimes(lexer, 2), init_token(minishell, ">>", T_DGREAT));
	else if (lexer->src[lexer->cur_pos] == '\n')
		return (advance(lexer), init_token(minishell, "\n", T_NEWLINE));
	else if (lexer->src[lexer->cur_pos] == '|')
		return (advance(lexer), init_token(minishell, "|", T_PIPE));
	else if (lexer->src[lexer->cur_pos] == '(')
		return (advance(lexer), init_token(minishell, "(", T_LPAR));
	else if (lexer->src[lexer->cur_pos] == ')')
		return (advance(lexer), init_token(minishell, ")", T_RPAR));
	else if (lexer->src[lexer->cur_pos] == '<')
		return (advance(lexer), init_token(minishell, "<", T_LESS));	
	else if (lexer->src[lexer->cur_pos] == '>')
		return (advance(lexer), init_token(minishell, ">", T_GREAT));
	else if (ft_ischar(lexer->src[lexer->cur_pos]))
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

int	lexer(t_ms *minishell, char *s)
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
	if (!error_in_lexer(minishell))
		return(0);
	return(1);
}
