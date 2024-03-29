/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 15:45:41 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/29 16:55:47 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

void	free_quote_state(t_ms *ms, t_dw *dw)
{
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

/*
Renseigne sur le nb de quotes presents a la suite, lorqu'on parse une value
NULL, il faut advance_ntimes avec le nb de quotes
*/
int	quote_size(t_ms *ms)
{
	int	i;

	i = 0;
	while (ms->lexer->src[ms->lexer->cur_pos + i] == '\'' || \
	ms->lexer->src[ms->lexer->cur_pos + i] == '\"')
		i++;
	return (i);
}

int	ft_isand(char *src, int cur_posi)
{
	if (src[cur_posi] && src[cur_posi] == '&' && \
	src[cur_posi + 1] && src[cur_posi + 1] == '&')
		return (1);
	return (0);
}

size_t	ft_strlen_lex(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*quotes_strjoin(char *s1, char *s2, int size)
{
	char	*dest;
	int		i;
	int		j;

	dest = malloc(sizeof(char) * ft_strlen_lex(s1) + ft_strlen_lex(s2) + 1);
	if (!dest)
		return (free(s1), NULL);
	i = 0;
	while (((char *)s1)[i])
	{
		dest[i] = s1[i];
		i++;
	}
	j = 0;
	while (((char *)s2)[j] && j < size)
	{
		dest[i + j] = s2[j];
		j++;
	}
	dest[i + j] = '\0';
	if (s1)
		free(s1);
	return (dest);
}
