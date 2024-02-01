/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse_word2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:32:26 by mapoirie          #+#    #+#             */
/*   Updated: 2024/02/01 17:37:41 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

/*
Renseigne sur l'etat des quotes: 
si quote ouverte valeur renvoyee = 1
si quote fermee valeur renvoyee = 0
*/
int	qstate(int i)
{
	if (i == 0)
		return (1);
	if (i % 2 == 0)
		return (0);
	return (1);
}

int	else_qtype(int qtype)
{
	if (qtype == 34)
		return (39);
	else
		return (34);
}

/*
peut etre ms->lexer->src[ms->lexer->cur_pos + i] != qtype 
(pour que ce cas marche "$USER"$USER'$USER')
a ete enlever && ms->lexer->src[ms->lexer->cur_pos + i] == qtype
*/
int	switch_qtype(t_ms *ms, int i, int qtype)
{
	if (qstate(ms->nb_q) == 0 && \
	(ms->lexer->src[ms->lexer->cur_pos + i + 1] && \
	ms->lexer->src[ms->lexer->cur_pos + i + 1] == else_qtype(qtype)))
		qtype = else_qtype(qtype);
	return (qtype);
}

int	count_dol_chars_in(t_ms *ms, int i)
{
	int	count;

	count = 0;
	if (ms->lexer->src[ms->lexer->cur_pos + i]
		&& ms->lexer->src[ms->lexer->cur_pos + i] == '?')
		return (1);
	while (ms->lexer->src[ms->lexer->cur_pos + i] && \
	(((ms->lexer->src[ms->lexer->cur_pos + i] >= '0' && \
	ms->lexer->src[ms->lexer->cur_pos + i] <= '9') || \
	(ms->lexer->src[ms->lexer->cur_pos + i] >= 'A' && \
	ms->lexer->src[ms->lexer->cur_pos + i] <= 'Z') || \
	(ms->lexer->src[ms->lexer->cur_pos + i] >= 'a' && \
	ms->lexer->src[ms->lexer->cur_pos + i] <= 'z')) || \
	ms->lexer->src[ms->lexer->cur_pos + i] == '_'))
	{
		i++;
		count++;
	}
	return (count);
}

int	count_dol_chars_out(t_ms *ms, int i)
{
	int	count;

	count = 0;
	if (ms->lexer->src[ms->lexer->cur_pos + i]
		&& ms->lexer->src[ms->lexer->cur_pos + i] == '?')
		return (1);
	while (ms->lexer->src[ms->lexer->cur_pos + i] && \
	(((ms->lexer->src[ms->lexer->cur_pos + i] >= '0' && \
	ms->lexer->src[ms->lexer->cur_pos + i] <= '9') || \
	(ms->lexer->src[ms->lexer->cur_pos + i] >= 'A' && \
	ms->lexer->src[ms->lexer->cur_pos + i] <= 'Z') || \
	(ms->lexer->src[ms->lexer->cur_pos + i] >= 'a' && \
	ms->lexer->src[ms->lexer->cur_pos + i] <= 'z')) || \
	ms->lexer->src[ms->lexer->cur_pos + i] == '_'))
	{
		i++;
		count++;
	}
	return (count);
}
