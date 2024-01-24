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



int	update_lstwil(t_wil **wil, t_list *w)
{
//	dprintf(2, "update lst dol\n");
	if (!(*wil) && w)
	{
//		dprintf(2, "calloc\n");
		*wil = ft_calloc(1, sizeof(t_wil));
		if (!(*wil))
			return (1);
		(*wil)->w = NULL;
	}
	if (w)
		ft_lstadd_back(&(*wil)->w, w);
	return (0);
}


void	update_lstwil_in(t_ms *ms, int qtype, int i, t_wil **wil)
{
	t_list	*w;

	w = NULL;
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
//	ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 1) &&
		(ms->lexer->src[ms->lexer->cur_pos + i] != qtype))
	{
		// if (ms->lexer->src[ms->lexer->cur_pos + i] == '*')
		// {
			w = ft_lstnew_int(1);
			if (!w) // a verifier
				(ft_lstfree(&w), prefree_minishell(ms, NULL));
			// {
			// 	ms->exit_code = 134;
			// 	free_minishell(ms, 1);
			// }
		// }
		//ft_lstadd_back(dol, new);
		if (update_lstwil(wil, w) == 1)
			(ft_lstfree(&w), prefree_minishell(ms, NULL));
	}
}

int	update_lstwil_out(t_ms *ms, int i, int j, t_wil **wil)
{
//	dprintf(2, "lst dol out\n");
	t_list	*w;

	w = NULL;
	(void)j;
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
	ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 0) && (ms->lexer->src \
	[ms->lexer->cur_pos + i] != '\'' && ms->lexer->src[ms->lexer->cur_pos + i] \
	!= '\"'))
	{
		if (ms->lexer->src[ms->lexer->cur_pos + i] == '*')
		{
			w = ft_lstnew_int(0);
			if (!w)// a verifier
				(ft_lstfree(&w), prefree_minishell(ms, NULL));
//			{
//				ms->exit_code = 134;
//				free_minishell(ms, 1);
//			}
		}
		else
		{
			w = ft_lstnew_int(1);
			if (!w)// a verifier
				(ft_lstfree(&w), prefree_minishell(ms, NULL));			
		}
//		ft_lstadd_back(dol, new);
		if (update_lstwil(wil, w) == 1)
			(ft_lstfree(&w), prefree_minishell(ms, NULL));// a verifier
	}
//	dprintf(2, "sortie lst dol out\n");
	return (0);
}

t_dw	*init_dw(t_ms *ms)
{
	t_dw	*dw;

	dw = NULL;
	dw = ft_calloc(1, sizeof(t_dw));
	if (!dw)// a verifier
		free_minishell(ms, 255);
	return (dw);
}

int big_while(t_ms *ms)
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
t_token	*parse_quotes_word(t_ms *ms, int qtype, char *value)
{
	// t_dol	*dol;
	t_dw	*dw;

	// dol = NULL;
	dw = init_dw(ms);
	// while (ms->lexer->src[ms->lexer->cur_pos + ms->i] && (qstate(ms->nb_q) ==
	// 1 || (qstate(ms->nb_q) == 0 && ft_ischar(ms->lexer->src[ms->lexer->cur_pos
	// + ms->i], 0) && !ft_isand(ms->lexer->src, ms->lexer->cur_pos + ms->i))))
	// dprintf(2, "qtype --> %d\n", qtype);
	while (big_while(ms))
	{
		if (ms->lexer->src[ms->lexer->cur_pos + ms->i] == qtype)
			ms->nb_q++;
		if (qstate(ms->nb_q) == 1)//ouvert
		{
			// dprintf(2, "ouvert\n");
			update_lstwil_in(ms, qtype, ms->i, &dw->wil);
			update_lstdol_in(ms, qtype, ms->i, &dw->dol);
			value = quote_state_open(ms, qtype, ms->i, value);
			// printf("i = %d qtype = %d char = %c \n", ms->i, qtype, ms->lexer->src[ms->lexer->cur_pos + ms->i]);
		}
		if (qstate(ms->nb_q) == 0)//ferme
		{
			// dprintf(2, "ferme\n");
			update_lstwil_out(ms, ms->i, 1, &dw->wil);
			ms->j = update_lstdol_out(ms, ms->i, 1, &dw->dol);
			qtype = case_w_dol(ms, qtype, &value);
			qtype = switch_qtype(ms, ms->i, qtype);
			// printf("i = %d qtype = %d char = %c\n", ms->i, qtype, ms->lexer->src[ms->lexer->cur_pos + ms->i]);
		}
		ms->i++;
	}
	if (!value && qstate(ms->nb_q) == 0)
		ms->i = case_value_null(ms, &value);
	return (advntimes(ms->lexer, ms->i), init_tokwdol(ms, value, T_WORD, dw));
}
