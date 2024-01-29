#include "../../include/lexer.h"
#include "../libft/libft.h"

int	update_lstdol(t_dol **dol, t_list *d, t_list *c)
{
	if (!(*dol) && d && c)
	{
		*dol = ft_calloc(1, sizeof(t_dol));// c'est verifie 1
		if (!(*dol))
			return (1);
		(*dol)->d = NULL;
		(*dol)->c = NULL;
	}
	if (d)
		ft_lstadd_back(&(*dol)->d, d);
	if (c)
		ft_lstadd_back(&(*dol)->c, c);
	return (0);
}

void	free_update_lstdol(t_ms *ms, t_dw *dw, t_list *d, t_list *c)
{
	if (d)
		ft_lstfree(&d);
	if (c)
		ft_lstfree(&c);
	if (dw && dw->dol && dw->dol->d)
		ft_lstfree(&dw->dol->d);
	if (dw && dw->dol && dw->dol->c)
		ft_lstfree(&dw->dol->c);
	if (dw && dw->wil && dw->wil->w)
		ft_lstfree(&dw->wil->w);
	if (dw && dw->dol)
		free(dw->dol);
	if (dw && dw->wil)
		free(dw->wil);
	if (dw)
		free(dw);
	(free(ms->value), prefree_minishell(ms, NULL));
}

void	update_lstdol_in(t_ms *ms, int qtype, int i, t_dw *dw)
{
	t_list	*d;
	t_list	*c;

	d = NULL;
	c = NULL;
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
	(ms->lexer->src[ms->lexer->cur_pos + i] != qtype))
	{
		if (ms->lexer->src[ms->lexer->cur_pos + i] == '$' && qtype == '\'')
		{
			d = ft_lstnew_int(0);
			c = ft_lstnew_int(0);
			if (!d || !c) // c'est verifie 1
				free_update_lstdol(ms, dw, d, c);
		}
		else if (ms->lexer->src[ms->lexer->cur_pos + i] == '$' && qtype == '\"')
		{
			d = ft_lstnew_int(1);
			c = ft_lstnew_int(count_dol_chars_in(ms, i + 1));
			if (!d || !c) // c'est verifie 1
				free_update_lstdol(ms, dw, d, c);
		}
		if (update_lstdol(&dw->dol, d, c) == 1)
			free_update_lstdol(ms, dw, d, c);// c'est verfie 1
	}
}

int	update_lstdol_out(t_ms *ms, int i, int j, t_dw *dw)
{
	t_list	*d;
	t_list	*c;

	d = NULL;
	c = NULL;
	if (ms->lexer->src[ms->lexer->cur_pos + i] && ft_ischar(ms->lexer \
	->src[ms->lexer->cur_pos + i], 0) && (ms->lexer->src[ms->lexer-> \
	cur_pos + i] != '\'' && ms->lexer->src[ms->lexer->cur_pos + i] != '\"'))
	{
		if (ms->lexer->src[ms->lexer->cur_pos + i] == '$')
		{
			while (ms->lexer->src[ms->lexer->cur_pos + i + j] == '$')
				j++;
			if (ms->lexer->src[ms->lexer->cur_pos + i + j] == '\'' || \
			ms->lexer->src[ms->lexer->cur_pos + i + j] == '\"')
				return (j);
			d = ft_lstnew_int(1);
			c = ft_lstnew_int(count_dol_chars_out(ms, i + 1));
			if (!d || !c)
				free_update_lstdol(ms, dw, d, c);// c'est verifie 1	
		}
		if (update_lstdol(&dw->dol, d, c) == 1)
			free_update_lstdol(ms, dw, d, c);// c'est verifie 1
	}
	return (0);
}

int	case_w_dol(t_ms *ms, int qtype, t_dw *dw)
{
	if (ms->j > 0)
	{
		while (ms->lexer->src[ms->lexer->cur_pos + ms->i + ms->j]
			&& (ms->lexer->src[ms->lexer->cur_pos + ms->i + ms->j] == '\''
				|| ms->lexer->src[ms->lexer->cur_pos + ms->i + ms->j] == '\"'))
		{
			if (ms->lexer->src[ms->lexer->cur_pos + ms->i + ms->j + 1]
				&& ms->lexer->src[ms->lexer->cur_pos + ms->i + ms->j]
				== ms->lexer->src[ms->lexer->cur_pos + ms->i + ms->j + 1])
					ms->j++;
			else
				break ;
		}
		ms->i += ms->j;
		qtype = ms->lexer->src[ms->lexer->cur_pos + ms->i];
		ms->nb_q++;
		quote_state_open(ms, qtype, ms->i, dw);
	}
	else
		quote_state_close(ms, ms->i, dw);
	return (qtype);
}
