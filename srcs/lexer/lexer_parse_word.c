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


int	case_value_null(t_ms *ms, char **value)
{
	int	i;

	i = 0;
	*value = ft_calloc(1, sizeof(char));
	if (!value)
		free_minishell(ms, 1);
	i = quote_size(ms);
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
t_token	*parse_quotes_word(t_ms *ms, int qtype, char *value)
{
	t_list	*dol;

	dol = NULL;
	while (ms->lexer->src[ms->lexer->cur_pos + ms->i] && (qstate(ms->nb_q) == \
	1 || (qstate(ms->nb_q) == 0 && ft_ischar(ms->lexer->src[ms->lexer->cur_pos \
	+ ms->i], 0) && !ft_isand(ms->lexer->src, ms->lexer->cur_pos + ms->i))))
	{
		if (ms->lexer->src[ms->lexer->cur_pos + ms->i] == qtype)
			ms->nb_q++;
		if (qstate(ms->nb_q) == 1)//ouvert
		{
			update_lstdol_in(ms, qtype, ms->i, &dol);
			value = quote_state_open(ms, qtype, ms->i, value);
		}
		if (qstate(ms->nb_q) == 0)//ferme
		{	
			ms->j = update_lstdol_out(ms, ms->i, 1, &dol);
			qtype = case_w_dol(ms, qtype, &value);
			qtype = switch_qtype(ms, ms->i, qtype);
		}
		ms->i++;
	}
	if (!value && qstate(ms->nb_q) == 0)
		ms->i = case_value_null(ms, &value);
	return (advntimes(ms->lexer, ms->i), init_tokwdol(ms, value, T_WORD, dol));
}
