#include "../../include/lexer.h"
#include "../libft/libft.h"

int	else_qtype(int qtype)
{
	if (qtype == 34)
		return (39);
	else
		return (34);
}

int	switch_qtype(t_ms *ms, int i, int qtype)
{
	//peut etre ms->lexer->src[ms->lexer->cur_pos + i] != qtype (pour que ce cas marche "$USER"$USER'$USER')
	if (qstate(ms->nb_q) == 0 /*&& ms->lexer->src[ms->lexer->cur_pos + i] == qtype*/ && \
	(ms->lexer->src[ms->lexer->cur_pos + i + 1] && \
	ms->lexer->src[ms->lexer->cur_pos + i + 1] == else_qtype(qtype)))
		qtype = else_qtype(qtype);
	// dprintf(2, "qqq-> %d\n", qtype);
	return (qtype);
}

int	case_w_dol(t_ms *ms, int qtype, char **value)
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
		// dprintf(2, "qtype ===> %d\n", qtype);//temp
		ms->nb_q++;
		// dprintf(2, "i = %d, qtype = %d, nb_q = %d\n", ms->i, qtype, ms->nb_q);
		// dprintf(2, "1j = %d\n", ms->j);
		*value = quote_state_open(ms, qtype, ms->i, *value);
		// dprintf(2, "value = %s\n", (char *)value);
	}
	else
		*value = quote_state_close(ms, ms->i, *value);
	return (qtype);
}

int	count_dol_chars_in(t_ms *ms, int i)
{
	int	count;

	count = 0;
	while (ms->lexer->src[ms->lexer->cur_pos + i]
		&& ms->lexer->src[ms->lexer->cur_pos + i] != '$'
		&& ms->lexer->src[ms->lexer->cur_pos + i] != '='
		&& ms->lexer->src[ms->lexer->cur_pos + i] != '\"'
		&& ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 0)
		&& !ft_isand(ms->lexer->src, ms->lexer->cur_pos + i)) // A VERIFIER
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
		&& ms->lexer->src[ms->lexer->cur_pos + i] != '$'
		&& ms->lexer->src[ms->lexer->cur_pos + i] != '='
		&& ms->lexer->src[ms->lexer->cur_pos + i] != '\"'
		&& ms->lexer->src[ms->lexer->cur_pos + i] != '\''
		&& ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 0)
		&& !ft_isand(ms->lexer->src, ms->lexer->cur_pos + i)) // A VERIFIER
	{
		i++;
		count++;
	}
	return (count);
}

int	update_lstdol(t_dol **dol, t_list *d, t_list *c)
{
//	dprintf(2, "update lst dol\n");
	if (!(*dol) && d && c)
	{
//		dprintf(2, "calloc\n");
		*dol = ft_calloc(1, sizeof(t_dol));
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

void	update_lstdol_in(t_ms *ms, int qtype, int i, t_dol **dol)
{
	t_list	*d;
	t_list	*c;

	d = NULL;
	c = NULL;
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
//	ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 1) &&
		(ms->lexer->src[ms->lexer->cur_pos + i] != qtype))
	{
		if (ms->lexer->src[ms->lexer->cur_pos + i] == '$' && qtype == '\'')
		{
			d = ft_lstnew_int(0);
			c = ft_lstnew_int(0);
			if (!d || !c) // a verifier
				(ft_lstfree(&d), ft_lstfree(&c), prefree_minishell(ms, NULL));
			// {
			// 	ms->exit_code = 134;
			// 	free_minishell(ms, 1);
			// }		
		}
		else if (ms->lexer->src[ms->lexer->cur_pos + i] == '$' && qtype == '\"')
		{
			d = ft_lstnew_int(1);
			c = ft_lstnew_int(count_dol_chars_in(ms, i + 1));
			if (!d || !c) // a verifier
				(ft_lstfree(&d), ft_lstfree(&c), prefree_minishell(ms, NULL));
			// {
			// 	ms->exit_code = 134;
			// 	free_minishell(ms, 1);
			// }
		}
		//ft_lstadd_back(dol, new);
		if (update_lstdol(dol, d, c) == 1)
			(ft_lstfree(&d), ft_lstfree(&c), prefree_minishell(ms, NULL));
	}
}

int	update_lstdol_out(t_ms *ms, int i, int j, t_dol **dol)
{
//	dprintf(2, "lst dol out\n");
	t_list	*d;
	t_list	*c;

	d = NULL;
	c = NULL;
	if (ms->lexer->src[ms->lexer->cur_pos + i] && \
	ft_ischar(ms->lexer->src[ms->lexer->cur_pos + i], 0) && (ms->lexer->src \
	[ms->lexer->cur_pos + i] != '\'' && ms->lexer->src[ms->lexer->cur_pos + i] \
	!= '\"'))
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
				(ft_lstfree(&d), ft_lstfree(&c), prefree_minishell(ms, NULL));
//			{
//				ms->exit_code = 134;
//				free_minishell(ms, 1);
//			}
		}
//		ft_lstadd_back(dol, new);
		if (update_lstdol(dol, d, c) == 1)
			(ft_lstfree(&d), ft_lstfree(&c), prefree_minishell(ms, NULL));
	}
//	dprintf(2, "sortie lst dol out\n");
	return (0);
}
