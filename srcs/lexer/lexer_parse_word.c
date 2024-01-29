#include "../../include/lexer.h"
#include "../libft/libft.h"

/*
Lorsqu'on se trouve a la suite de quote fermee on parse que les lettres 
et les characters speciaux, et pas les quotes ou les espaces
*/
void	quote_state_close(t_ms *ms, int i, t_dw *dw)
{
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
	ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 0) && (ms->lexer->src \
	[ms->lexer->cur_pos + i] != '\'' && ms->lexer->src[ms->lexer->cur_pos + i] \
	!= '\"'))
	{
		if (!ms->value)
		{
			ms->value = ft_calloc(2, sizeof(char));
			if (!ms->value)
				free_quote_state(ms, dw);
			ft_strlcpy(ms->value, &ms->lexer->src[ms->lexer->cur_pos + i], 2);
		}
		else if (ms->value)
		{
			ms->value = quotes_strjoin(ms->value, \
			&ms->lexer->src[ms->lexer->cur_pos + i], 1);
			if (!ms->value)
				free_quote_state(ms, dw);
		}
	}
}

/*
Lorsqu'on se trouve a la suite de quote ouverte on parse les lettres, les
character speciaux, les quotes qui ne sont pas du meme type que la 1ere
quote rencontree et les espaces
L'indicateur dol mi a 1 renseigne sur le fait qu'il ne faudra pas expand
la variable apres le $ 
*/
void	quote_state_open(t_ms *ms, int qtype, int i, t_dw *dw)
{
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
	ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 1) && \
	(ms->lexer->src[ms->lexer->cur_pos + i] != qtype))
	{
		if (!ms->value)
		{
			ms->value = ft_calloc(2, sizeof(char));
			if (!ms->value)
				free_quote_state(ms, dw);
			ft_strlcpy(ms->value, &ms->lexer->src[ms->lexer->cur_pos + i], 2);
		}
		else if (ms->value)
		{
			ms->value = quotes_strjoin(ms->value, \
			&ms->lexer->src[ms->lexer->cur_pos + i], 1);
			if (!ms->value)
				free_quote_state(ms, dw);
		}
	}
}

int	case_value_null(t_ms *ms, t_dw *dw)
{
	int	i;

	i = 0;
	ms->value = ft_calloc(1, sizeof(char));
	if (!ms->value)
		free_quote_state(ms, dw);
	i = quote_size(ms);
	return (i);
}

int	big_while(t_ms *ms)
{
	if (ms->lexer->src[ms->lexer->cur_pos + ms->i] && (qstate(ms->nb_q) == \
	1 || (qstate(ms->nb_q) == 0 && ft_ischar(ms->lexer->src[ms->lexer->cur_pos \
	+ ms->i], 0) && !ft_isand(ms->lexer->src, ms->lexer->cur_pos + ms->i))))
		return (1);
	return (0);
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
t_token	*parse_quotes_word(t_ms *ms, int qtype)
{
	t_dw	*dw;

	dw = init_dw(ms);
	while (big_while(ms))
	{
		if (ms->lexer->src[ms->lexer->cur_pos + ms->i] == qtype)
			ms->nb_q++;
		if (qstate(ms->nb_q) == 1)
		{
			update_lstwil_in(ms, qtype, ms->i, dw);
			update_lstdol_in(ms, qtype, ms->i, dw);
			quote_state_open(ms, qtype, ms->i, dw);
		}
		if (qstate(ms->nb_q) == 0)
		{
			update_lstwil_out(ms, ms->i, dw);
			ms->j = update_lstdol_out(ms, ms->i, 1, dw);
			qtype = case_w_dol(ms, qtype, dw);
			qtype = switch_qtype(ms, ms->i, qtype);
		}
		ms->i++;
	}
	if (!ms->value && qstate(ms->nb_q) == 0)
		ms->i = case_value_null(ms, dw);
	return (advntimes(ms->lexer, ms->i), init_tokwdol(ms, T_WORD, dw));
}
