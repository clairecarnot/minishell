/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse_word.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:30:04 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/23 11:33:34 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

// t_token	*parse_following_quotes(t_ms *ms, t_lexer *lexer, int qtype)
// {	
// 	int	i;
// 	int	j;
// 	char	*value;

// 	i = 0;
// 	j = 0;
// 	while (lexer->src[lexer->cur_pos + i] && lexer->src[lexer->cur_pos + i] == qtype)
// 		i++;
// 	if (!lexer->src[lexer->cur_pos + i] || \
// 	(lexer->src[lexer->cur_pos + i] && lexer->src[lexer->cur_pos + i] == ' ' && i % 2 == 0))//cas """"  hey  """" -> T_WORD1="" T_WORD2="hey" T_WORD3=""
// 	{
// 		value = ft_calloc(1, sizeof(char));
// 		value = NULL;
// 		advance_ntimes(lexer, i);
// 		return (init_token(ms, value, T_WORD));
// 	}
// 	return (parse_quotes_word(ms, lexer, qtype));
// }

// //qtype = quote type
// t_token	*parse_quotes_word(t_ms *ms, t_lexer *lexer, int qtype)
// {
// 	int		i;
// 	int		q_nb;
// 	char	*value;

// 	i = 0;
// 	q_nb = 0;
// 	while (lexer->src[lexer->cur_pos + i] && lexer->src[lexer->cur_pos + i] == qtype)
// 	{
// 		i++;
// 		q_nb++;
// 	}
// 	while (lexer->src[lexer->cur_pos + i] && \
// 	ft_ischar(lexer->src[lexer->cur_pos + i], 1) && \
// 	lexer->src[lexer->cur_pos + i] != qtype)
// 		i++;
// 	value = ft_calloc(i, sizeof(char));
// 	if (!value)//verifier protec
// 		return (NULL);
// 	ft_strlcpy(value, &(lexer->src[lexer->cur_pos + q_nb]), i - q_nb + 1);//peut-etre a proteger
// 	while(lexer->src[lexer->cur_pos + i] && \
// 	(lexer->src[lexer->cur_pos + i] == '\'' || lexer->src[lexer->cur_pos + i] == '\"'))
// 		i++;
// 	advance_ntimes(lexer, i);
// 	return (init_token(ms, value, T_WORD));
// }

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

char	*quotes_strjoin(char const *s1, char const *s2, int size)
{
	char	*dest;
	int		i;
	int		j;

	dest = malloc(sizeof(char) * ft_strlen_lex(s1) + ft_strlen_lex(s2) + 1);
	if (!dest)//verifier protec
		return (NULL);
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
	return (dest);
}

// if i est impair --> citation ouverte """ 0123
// if i est pair --> citation fermee "" 012
int	citation_state(int i)
{
	if (i == 0)
		return(1);//ouvert
	if (i % 2 == 0)//pair --> fermee --> 0
		return (0);
	return (1);//impar --> ouvert --> 1
}

// lorsqu'on arrive dans cette fonction on est au debut d'une quote et on a son type (simple ou double)
// tant qu'on est en l'interieur d'une citation ou tant qu'on est a l'exterieur de la citation mais
// qu'il n'y a pas d'espace on reste dans le meme token (des qu'il y a espace avec quotes et quand se trouve 
// en dehors d'une citation -> nouveau token)
// 1 = citation ouvert ou 0 = ferme
t_token	*parse_quotes_word(t_ms *ms, t_lexer *lexer, int qtype)
{
	int		i;
	int		nb_q;
	char	*value;

	i = 0;
	nb_q = 0;
	value = NULL;
	if (qtype)
	while (lexer->src[lexer->cur_pos + i] && \
	(citation_state(nb_q) == 1 || \
	(citation_state(nb_q) == 0 && lexer->src[lexer->cur_pos + i] != ' ')))
	{
		if (lexer->src[lexer->cur_pos + i] == qtype)
			nb_q++;
		if (citation_state(nb_q) == 1)//citation ouverte, on parse les lettres et les quotes
		{
			if (lexer->src[lexer->cur_pos + i] && \
			ft_ischar(lexer->src[lexer->cur_pos + i], 1) && \
			(lexer->src[lexer->cur_pos + i] != qtype))
			{
				if (!value)
				{
					value = ft_calloc(1, sizeof(char));//add protec
					ft_strlcpy(value, &lexer->src[lexer->cur_pos + i], 2);
				}
				else if (value)
				{
					value = quotes_strjoin(value, &lexer->src[lexer->cur_pos + i], 1);
					if (!value)//protect
						return(NULL);// ou NULL for error
				}
			}
		}
		if (citation_state(nb_q) == 0)//citation ferme, on parse que les lettres pas quotes
		{
			if (lexer->src[lexer->cur_pos + i] && \
			ft_ischar(lexer->src[lexer->cur_pos + i], 1) && \
			(lexer->src[lexer->cur_pos + i] != '\'' && lexer->src[lexer->cur_pos + i] != '\"'))
			{
				if (!value)
				{
					value = ft_calloc(1, sizeof(char));//add protec
					ft_strlcpy(value, &lexer->src[lexer->cur_pos + i], 2);
				}
				else if (value)
				{
					value = quotes_strjoin(value, &lexer->src[lexer->cur_pos + i], 1);
					if (!value)//protect
						return(NULL);// ou NULL for error
				}
			}
		}
		i++;
	}
	if (!value && citation_state(nb_q) == 0)
	{
		i = 0;
		while (lexer->src[lexer->cur_pos + i] == '\'' || lexer->src[lexer->cur_pos + i] == '\"')
			i++;
		advance_ntimes(lexer, i);
	}
	else
		advance_ntimes(lexer, i);
	return (init_token(ms, value, T_WORD));
}

t_token	*parse_word(t_ms *minishell, t_lexer *lexer, int i)
{
	char	*value;
	// int		i;

	// i = 0;
	while (lexer->src[lexer->cur_pos + i] && \
	ft_ischar(lexer->src[lexer->cur_pos + i], 0))
		i++;
	value = ft_calloc(i + 1, sizeof(char));
	if (!value)
		return (NULL);
	ft_strlcpy(value, &(lexer->src[lexer->cur_pos]), i + 1);//peut etre a proteger
	advance_ntimes(lexer, i);
	return (init_token(minishell, value, T_WORD));
}
