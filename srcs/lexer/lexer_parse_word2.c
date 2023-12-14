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
	if (qstate(ms->nb_q) == 0 && ms->lexer->src[ms->lexer->cur_pos + i] == qtype && \
	(ms->lexer->src[ms->lexer->cur_pos + i + 1] && \
	ms->lexer->src[ms->lexer->cur_pos + i + 1] == else_qtype(qtype)))
		qtype = else_qtype(qtype);
	return (qtype);
}

int	case_w_dol(t_ms *ms, int qtype, char **value)
{
	if (ms->j > 0)
	{
		ms->i += ms->j;
		qtype = ms->lexer->src[ms->lexer->cur_pos + ms->i];
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
			if (!new) // a verifier
				prefree_minishell(ms, NULL);
			// {
			// 	ms->exit_code = 134;
			// 	free_minishell(ms, 1);
			// }		
		}
		else if (ms->lexer->src[ms->lexer->cur_pos + i] == '$' && qtype == '\"')
		{
			new = ft_lstnew_int(1);
			if (!new) // a verifier
				prefree_minishell(ms, NULL);
			// {
			// 	ms->exit_code = 134;
			// 	free_minishell(ms, 1);
			// }
		}
		ft_lstadd_back(dol, new);
	}
}

int	update_lstdol_out(t_ms *ms, int i, int j, t_list **dol)
{
	t_list	*new;

	new = NULL;
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
			new = ft_lstnew_int(1);
			if (!new)
			{
				ms->exit_code = 134;
				free_minishell(ms, 1);
			}
		}
		ft_lstadd_back(dol, new);
	}
	return (0);
}
