/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:30:06 by mapoirie          #+#    #+#             */
/*   Updated: 2024/02/01 17:38:00 by mapoirie         ###   ########.fr       */
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
	lexer->next_pos = 1;
	lexer->token_lst = NULL;
	return (lexer);
}

t_token	*init_token(t_ms *ms, char *value, t_type type)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		free_minishell(ms, 255);
	token->type = type;
	token->value = value;
	token->next_token = NULL;
	token->dol = NULL;
	token->wil = NULL;
	return (token);
}

t_token	*init_tokwdol(t_ms *minishell, t_type type, t_dw *dw)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		free_quote_state(minishell, dw);
	token->type = type;
	token->value = minishell->value;
	token->next_token = NULL;
	token->dol = dw->dol;
	token->wil = dw->wil;
	free(dw);
	minishell->value = NULL;
	return (token);
}
