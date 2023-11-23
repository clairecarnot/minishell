/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse_word.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:30:04 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/23 17:56:55 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

// if i est impair --> citation ouverte """..  => 1
// if i est pair   --> citation fermee ""..    => 0
int	qstate(int i)
{
	if (i == 0)
		return (1);
	if (i % 2 == 0)
		return (0);
	return (1);
}

//citation ferme, on parse que les lettres pas quotes
char	*quote_state_close(t_ms *ms, int i, char *value)
{
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
	ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 1) && (ms->lexer->src \
	[ms->lexer->cur_pos + i] != '\'' && ms->lexer->src[ms->lexer->cur_pos + i] != '\"'))
	{
		if (!value)
		{
			value = ft_calloc(2, sizeof(char));
			if (!value)
				free_minishell(ms, 1);
			ft_strlcpy(value, &ms->lexer->src[ms->lexer->cur_pos + i], 2);
		}
		else if (value)
		{
			value = quotes_strjoin(value, &ms->lexer->src[ms->lexer->cur_pos + i], 1);
			if (!value)
				free_minishell(ms, 1);
		}
	}
	return (value);
}

//citation ouverte, on parse les lettres et les quotes
char	*quote_state_open(t_ms *ms, int qtype, int i, char *value)
{
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
	ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 1) && \
	(ms->lexer->src[ms->lexer->cur_pos + i] != qtype))
	{
		if (!value)
		{
			value = ft_calloc(2, sizeof(char));
			if (!value)
				free_minishell(ms, 1);
			ft_strlcpy(value, &ms->lexer->src[ms->lexer->cur_pos + i], 2);
		}
		else if (value)
		{
			value = quotes_strjoin(value, &ms->lexer->src[ms->lexer->cur_pos + i], 1);
			if (!value)
				free_minishell(ms, 1);
		}
	}
	return (value);
}

int	quote_size(t_ms *ms)
{
	int i;

	i = 0;
	while (ms->lexer->src[ms->lexer->cur_pos + i] == '\'' || \
	ms->lexer->src[ms->lexer->cur_pos + i] == '\"')
		i++;
	return (i);
}

// lorsqu'on arrive dans cette fonction on est au debut d'une quote et on a son type (simple ou double)
// tant qu'on est en l'interieur d'une citation ou tant qu'on est a l'exterieur de la citation mais
// qu'il n'y a pas d'espace on reste dans le meme token (des qu'il y a espace avec quotes et quand se trouve 
// en dehors d'une citation -> nouveau token)
// 1 = citation ouvert ou 0 = ferme
t_token	*parse_quotes_word(t_ms *ms, int qtype, int nb_q)
{
	int		i;
	char	*value;

	i = 0;
	value = NULL;
	while (ms->lexer->src[ms->lexer->cur_pos + i] && (qstate(nb_q) == 1 || \
	(qstate(nb_q) == 0 && ms->lexer->src[ms->lexer->cur_pos + i] != ' ')))
	{
		if (ms->lexer->src[ms->lexer->cur_pos + i] == qtype)
			nb_q++;
		if (qstate(nb_q) == 1)
			value = quote_state_open(ms, qtype, i, value);
		if (qstate(nb_q) == 0)
			value = quote_state_close(ms, i, value);
		i++;
	}
	if (!value && qstate(nb_q) == 0)
	{
		value = ft_calloc(1, sizeof(char));
		if (!value)
			free_minishell(ms, 1);
		i = quote_size(ms);
	}
	advance_ntimes(ms->lexer, i);
	return (init_token(ms, value, T_WORD));
}

// t_token	*parse_word(t_ms *minishell, t_lexer *lexer, int i)
// {
// 	char	*value;

// 	while (lexer->src[lexer->cur_pos + i] && \
// 	ft_ischar(lexer->src[lexer->cur_pos + i], 0))
// 		i++;
// 	value = ft_calloc(i + 1, sizeof(char));
// 	if (!value)//changer protect
// 		return (NULL);//free minishell
// 	ft_strlcpy(value, &(lexer->src[lexer->cur_pos]), i + 1);
// 	advance_ntimes(lexer, i);
// 	return (init_token(minishell, value, T_WORD));
// }
