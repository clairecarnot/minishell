/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:34:23 by ccarnot           #+#    #+#             */
/*   Updated: 2023/11/13 10:41:19 by mapoirie         ###   ########.fr       */
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
	printf("word = %s\n", value);
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
		return (advance(lexer), init_token(minishell, "\n", T_NEWLINE));//a verifier car ne marche pas
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
	else if (lexer->src[lexer->cur_pos] == 39)// single quote
		return (parse_quotes_word(minishell, lexer, 39));
	else if (lexer->src[lexer->cur_pos] == 34)// double quotes
		return (parse_quotes_word(minishell, lexer, 34));
	else if (ft_ischar(lexer->src[lexer->cur_pos], 0))
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
	if (error_in_lexer(minishell) != 0)//if there's an error
		return(1);
	return(0);
}
