/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse_word_wil.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:39:24 by mapoirie          #+#    #+#             */
/*   Updated: 2024/02/01 17:39:25 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

int	update_lstwil(t_wil **wil, t_list *w)
{
	if (!(*wil) && w)
	{
		*wil = ft_calloc(1, sizeof(t_wil));
		if (!(*wil))
			return (1);
		(*wil)->w = NULL;
	}
	if (w)
		ft_lstadd_back(&(*wil)->w, w);
	return (0);
}

void	free_update_lstwil(t_ms *ms, t_dw *dw, t_list *w)
{
	if (w)
		ft_lstfree(&w);
	if (dw && dw->wil && dw->wil->w)
		ft_lstfree(&dw->wil->w);
	if (dw && dw->dol && dw->dol->d)
		ft_lstfree(&dw->dol->d);
	if (dw && dw->dol && dw->dol->c)
		ft_lstfree(&dw->dol->c);
	if (dw && dw->dol)
		free(dw->dol);
	if (dw && dw->wil)
		free(dw->wil);
	if (dw)
		free(dw);
	(free(ms->value), prefree_minishell(ms, NULL));
}

void	update_lstwil_in(t_ms *ms, int qtype, int i, t_dw *dw)
{
	t_list	*w;

	w = NULL;
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
		(ms->lexer->src[ms->lexer->cur_pos + i] != qtype))
	{
		w = ft_lstnew_int(1);
		if (!w)
			free_update_lstwil(ms, dw, w);
		if (update_lstwil(&dw->wil, w) == 1)
			free_update_lstwil(ms, dw, w);
	}
}

int	update_lstwil_out(t_ms *ms, int i, t_dw *dw)
{
	t_list	*w;

	w = NULL;
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
	ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 0) && (ms->lexer->src \
	[ms->lexer->cur_pos + i] != '\'' && ms->lexer->src[ms->lexer->cur_pos + i] \
	!= '\"'))
	{
		if (ms->lexer->src[ms->lexer->cur_pos + i] == '*')
		{
			w = ft_lstnew_int(0);
			if (!w)
				free_update_lstwil(ms, dw, w);
		}
		else
		{
			w = ft_lstnew_int(1);
			if (!w)
				free_update_lstwil(ms, dw, w);
		}
		if (update_lstwil(&dw->wil, w) == 1)
			free_update_lstwil(ms, dw, w);
	}
	return (0);
}

t_dw	*init_dw(t_ms *ms)
{
	t_dw	*dw;

	dw = ft_calloc(1, sizeof(t_dw));
	if (!dw)
		free_minishell(ms, 255);
	return (dw);
}
