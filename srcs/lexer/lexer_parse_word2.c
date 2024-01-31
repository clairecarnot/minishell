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
	while (ms->lexer->src[ms->lexer->cur_pos + i]
		&& (((ms->lexer->src[ms->lexer->cur_pos + i] >= '0' && ms->lexer->src[ms->lexer->cur_pos + i] <= '9')
		|| (ms->lexer->src[ms->lexer->cur_pos + i] >= 'A' && ms->lexer->src[ms->lexer->cur_pos + i] <= 'Z') 
		|| (ms->lexer->src[ms->lexer->cur_pos + i] >= 'a' && ms->lexer->src[ms->lexer->cur_pos + i] <= 'z'))
		|| ms->lexer->src[ms->lexer->cur_pos + i] == '_')) 
		// && ms->lexer->src[ms->lexer->cur_pos + i] != '$'
		// && ms->lexer->src[ms->lexer->cur_pos + i] != '='
		// && ms->lexer->src[ms->lexer->cur_pos + i] != '\"'
		// && ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 0)
		// && !ft_isand(ms->lexer->src, ms->lexer->cur_pos + i))
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
	while (ms->lexer->src[ms->lexer->cur_pos + i]
		&& (((ms->lexer->src[ms->lexer->cur_pos + i] >= '0' && ms->lexer->src[ms->lexer->cur_pos + i] <= '9')
		|| (ms->lexer->src[ms->lexer->cur_pos + i] >= 'A' && ms->lexer->src[ms->lexer->cur_pos + i] <= 'Z') 
		|| (ms->lexer->src[ms->lexer->cur_pos + i] >= 'a' && ms->lexer->src[ms->lexer->cur_pos + i] <= 'z'))
		|| ms->lexer->src[ms->lexer->cur_pos + i] == '_')) 
		/*&& ms->lexer->src[ms->lexer->cur_pos + i] != '$'
		&& ms->lexer->src[ms->lexer->cur_pos + i] != '='
		&& ms->lexer->src[ms->lexer->cur_pos + i] != '\"'
		&& ms->lexer->src[ms->lexer->cur_pos + i] != '\''
		&& ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 0)
		&& !ft_isand(ms->lexer->src, ms->lexer->cur_pos + i))*/
	{
		i++;
		count++;
	}
	return (count);
}
