/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:30:06 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/26 09:24:57 by ccarnot          ###   ########.fr       */
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
	lexer->tok_count = 0;
	return (lexer);
}

t_token	*init_token(t_ms *ms, char *value, t_type type)
{
	t_token	*token;

	// dprintf(2, "ms->a = %d\n", minishell->a);
	// minishell->a++;
	// if (minishell->a > 3)
	// 	token = NULL;
	// else
//	token = ft_calloc(1, sizeof(t_token));
	token = NULL;
	if (!token)
	{
		// if (value)
		// 	free(value);
		// if (ms->w_ms)
		// {
		// 	dprintf(2, "ici\n");
		// 	if (ms->w_ms->w)
		// 	{
		// 		dprintf(2, "ici2\n");
		// 		ft_lstfree(&ms->w_ms->w);
		// 	}
		// 	free(ms->w_ms);
		// }
		free_minishell(ms, 255);
	}
	token->type = type;
	token->value = value;
	token->next_token = NULL;
	token->tok_nb = ms->lexer->tok_count;
	ms->lexer->tok_count++;
	token->dol = NULL;
	token->wil = NULL;
	return (token);
}

t_token	*init_tokwdol(t_ms *minishell, char *value, t_type type, t_dw *dw)
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
	token->tok_nb = minishell->lexer->tok_count;
	minishell->lexer->tok_count++;
	token->dol = dw->dol;
	token->wil = dw->wil;
	free(dw);
	return (token);
}
