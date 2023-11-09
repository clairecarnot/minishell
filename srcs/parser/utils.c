/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 11:52:28 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/09 16:24:28 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"

int	token_to_node(int type)
{
	if (type == T_WORD)
		return (CMD);
	else if (type == T_PIPE)
		return (PIPE);
	else if (type == T_AND_IF)
		return (AND_IF);
	else if (type == T_OR_IF)
		return (OR_IF);
	else if (type == T_LPAR)
		return (LPAR);
	else if (type == T_RPAR)
		return (RPAR);
	else if (type == T_LESS)
		return (LESS);
	else if (type == T_GREAT)
		return (GREAT);
	else if (type == T_DLESS)
		return (DLESS);
	else if (type == T_DGREAT)
		return (DGREAT);
	else if (type == T_NEWLINE)
		return (NEWLINE);
	else 
		return (END);
}

int	eat_token(t_ms *ms, t_type type)
{
	if (ms->cur_tok->type == type)
	{
		ms->cur_tok = ms->cur_tok->next_token;
		return (1);
	}
	return (0);
}

void	redirs_add_back(t_redirs **lst, t_redirs *new)
{
	t_redirs	*ptr;

	if (*lst == NULL)
		*lst = new;
	else
	{
		ptr = *lst;
		while (ptr->next_redir)
			ptr = ptr->next_redir;
		ptr->next_redir = new;
	}
}

t_redirs	*redirs_new(t_token *token, int type)
{
	t_redirs	*d;

	d = malloc(sizeof(t_redirs));
	if (!d)
		return (NULL);
	d->type = token_to_node(type);
	d->filename = ft_strdup(token->value);
	if (!d->filename)
		return (free(d), NULL);
	d->next_redir = NULL;
	return (d);
}