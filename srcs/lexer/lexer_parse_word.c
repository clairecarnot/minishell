/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse_word.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 14:30:04 by mapoirie          #+#    #+#             */
/*   Updated: 2023/12/07 17:37:49 by mapoirie         ###   ########.fr       */
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

/*
Lorsqu'on se trouve a la suite de quote fermee on parse que les lettres 
et les characters speciaux, et pas les quotes ou les espaces
*/
char	*quote_state_close(t_ms *ms, int i, char *value)
{
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
	ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 0) && (ms->lexer->src \
	[ms->lexer->cur_pos + i] != '\'' && ms->lexer->src[ms->lexer->cur_pos + i] \
	!= '\"'))
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
			value = quotes_strjoin(value, \
			&ms->lexer->src[ms->lexer->cur_pos + i], 1);
			if (!value)
				free_minishell(ms, 1);
		}
	}
	return (value);
}

/*
Lorsqu'on se trouve a la suite de quote ouverte on parse les lettres, les
character speciaux, les quotes qui ne sont pas du meme type que la 1ere
quote rencontree et les espaces
L'indicateur dol mi a 1 renseigne sur le fait qu'il ne faudra pas expand
la variable apres le $ 
*/
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
			value = quotes_strjoin(value, \
			&ms->lexer->src[ms->lexer->cur_pos + i], 1);
			if (!value)
				free_minishell(ms, 1);
		}
	}
	return (value);
}

void	update_lstdol_in(t_ms *ms, int qtype, int i, t_list **dol)
{
	t_list	*new;

	new = NULL;
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
	ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 1) && \
	(ms->lexer->src[ms->lexer->cur_pos + i] != qtype))
	{
		if (ms->lexer->src[ms->lexer->cur_pos + i] == '$' && qtype == '\'')
		{
			new = ft_lstnew_int(0);
			if (!new)
			{
				ms->exit_code = 134;
				free_minishell(ms, 1);
			}		
		}
		else if (ms->lexer->src[ms->lexer->cur_pos + i] == '$' && qtype == '\"')
		{
			new = ft_lstnew_int(1);
			if (!new)
			{
				ms->exit_code = 134;
				free_minishell(ms, 1);
			}		
		}
		ft_lstadd_back(dol, new);
	}
}

int	update_lstdol_out(t_ms *ms, int i, t_list **dol)
{
	int		j;
	t_list	*new;

	j = 1;
	new = NULL;
	// dprintf(2, "lstdol out\n");
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
	ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 0) && (ms->lexer->src \
	[ms->lexer->cur_pos + i] != '\'' && ms->lexer->src[ms->lexer->cur_pos + i] \
	!= '\"'))
	{
		// dprintf(2, "lstdol out 2\n");
		if (ms->lexer->src[ms->lexer->cur_pos + i] == '$')
		{
			// dprintf(2, "lstdol out 3\n");
			while(ms->lexer->src[ms->lexer->cur_pos + i + j] == '$')
				j++;
			if (ms->lexer->src[ms->lexer->cur_pos + i + j] == '\'' || ms->lexer->src[ms->lexer->cur_pos + i + j] == '\"')
				return (j);//
			
			new = ft_lstnew_int(1);
			if (!new)
			{
				
				ms->exit_code = 134;
				free_minishell(ms, 1);
			}
		}
		ft_lstadd_back(dol, new);
	}
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

/*
Cette fonction parse les mots avec ou sans quotes, pour en faire des T_WORD. 
S'il y a des quotes dans le mot, on envoie le type des 1eres quotes rencontrees 
(simples ou doubles). La grande boucle while se traduit tel que : tant que 
src[cur_pos + i] existe et que qstate (l'etat des quotes) est ouverte(1) OU 
est fermee(0) et src[cur_pos + i] n'est pas un espace, alors on parse le mot 
suivant l'etat ouvert ou ferme des quotes. Quand on sort du while, si !value 
on doit bien creer un arg vide pour que plus tard l'arg soit bien existant 
mais bien vide.
*/
t_token	*parse_quotes_word(t_ms *ms, int qtype, int nb_q, int i)
{
	char	*value;
	t_list	*dol;

	dol = NULL;
	value = NULL;
	// dprintf(2, "1\n");
	while (ms->lexer->src[ms->lexer->cur_pos + i] && (qstate(nb_q) == 1 || \
	(qstate(nb_q) == 0 && ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 0) \
	&& !ft_isand(ms->lexer->src, ms->lexer->cur_pos + i))))
	{
		if (ms->lexer->src[ms->lexer->cur_pos + i] == qtype)
			nb_q++;
		// dprintf(2, "2\n");
		if (qstate(nb_q) == 1)
		{
			update_lstdol_in(ms, qtype, i, &dol);
			value = quote_state_open(ms, qtype, i, value);
		}
		// dprintf(2, "3\n");
		if (qstate(nb_q) == 0)
		{
			i += update_lstdol_out(ms, i, &dol);
			// if (ms->lexer->src[ms->lexer->cur_pos + i] == '\'' || ms->lexer->src[ms->lexer->cur_pos + i] == '\"')
			// {
			// 	qtype = ms->lexer->src[ms->lexer->cur_pos + i] - '0';
			// 	nb_q++;
			// }
			// else
				value = quote_state_close(ms, i, value);
		}
		i++;
	}
	if (!value && qstate(nb_q) == 0)
	{
		value = ft_calloc(1, sizeof(char));
		if (!value)
			free_minishell(ms, 1);
		i = quote_size(ms);
	}
	// dprintf(2, "4\n");
	advance_ntimes(ms->lexer, i);
	return (init_token_wdol(ms, value, T_WORD, dol));
	// t_token *tok;
	
	// tok = init_token_wdol(ms, value, T_WORD, dol);
	// print_lst(tok->dol);
	// return (tok);
}
