#include "../../include/lexer.h"
#include "../../include/parser.h"

/*
 * redirs_add_back
 * Ajoute un element t_redirs a la fin d'une liste t_redirs
 */

void	redirs_add_back(t_redirs **lst, t_redirs *new)
{
	t_redirs	*ptr;

	if (*lst == NULL)
		*lst = new;
	else
	{
		ptr = *lst;
		while (ptr->next_redir)
			ptr = ptr->next_redir;
		ptr->next_redir = new;
	}
}

/*
 * redirs_new:
 * Cree une nouvelle structure t_redirs du type indique par le current token
 * Le nom du fichier vers lequel la redir se fait est indique par le next token
 * La next redir est initialisee a NULL
 */

t_redirs	*redirs_new(t_ms *ms, t_token *token, int type)
{
	t_redirs	*d;

	d = NULL;
	d = ft_calloc(1, sizeof(t_redirs));
	if (!d)
	{
		ms->exit_code = 255;
		return (NULL);
	}
	d->type = token_to_node(type);
	d->filename = NULL;
	d->filename = ft_strdup(token->value);
	if (!d->filename)
	{
		ms->exit_code = 255;
		return (free(d), NULL);
	}
	d->dol = NULL;
	d->next_redir = NULL;
	return (d);
}

/*
 * handle_red:
 * Cree une nouvelle structure t_redirs qui contient le nom du
 *  fichier correspondant a cette redirection. Cette structure est
 *  ajoutee a la liste des redirections de l'AST
 */

t_redirs	*handle_red(t_ms *ms, t_ast *new_ast)
{
	t_redirs	*new_redir;

	new_redir = NULL;
	if (ms->cur_tok->next_token->type != T_WORD)
		return (NULL);
	new_redir = redirs_new(ms, ms->cur_tok->next_token, ms->cur_tok->type);
	if (!new_redir)
		return (NULL);
	if (ms->cur_tok->next_token->dol)
		ft_doladd_back(ms, &new_redir->dol, ms->cur_tok->next_token->dol);
	if (ms->cur_tok->next_token->wil)
		ft_wiladd_back(ms, &new_ast->wil, ms->cur_tok->next_token->wil);
	redirs_add_back(&new_ast->redirs, new_redir);
	eat_token(ms, ms->cur_tok->type);
	eat_token(ms, ms->cur_tok->type);
	return (new_ast->redirs);
}

int	is_redirect(t_ms *ms, t_token *cur_tok)
{
	if (!cur_tok || (cur_tok->type < T_LESS || cur_tok->type > T_DGREAT))
		return (0);
	if (!cur_tok->next_token || cur_tok->next_token->type == T_EOF)
	{
		cur_tok->next_token->type = T_NEWLINE;
		return (0);
	}
	else if (cur_tok->next_token->type != T_WORD)
	{
		eat_token(ms, cur_tok->type);
		return (0);
	}
	return (1);
}
