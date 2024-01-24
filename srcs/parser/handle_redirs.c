/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:20:17 by mapoirie          #+#    #+#             */
/*   Updated: 2024/01/24 10:39:52 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// t_redirtype	*redirtype_new(t_ms *ms, t_node_type type)
// {
// 	t_redirtype	*tmp;

// 	tmp = NULL;
// 	tmp = ft_calloc(1, sizeof(t_redirtype));
// 	if (!tmp)
// 	{
// 		ms->exit_code = 255;
// 		return (NULL);
// 	}
// 	tmp->type = type;
// 	tmp->next = NULL;
// 	return (tmp);
// }

// void	rt_addback(t_redirtype **lst, t_redirtype *new)
// {
// 	t_redirtype	*ptr;

// 	if (*lst == 0x0)
// 		*lst = new;
// 	else
// 	{
// 		ptr = *lst;
// 		while (ptr->next)
// 			ptr = ptr->next;
// 		ptr->next = new;
// 	}
	// dprintf(2, "rt \n");
	// if (new->type == CMD)
	// 	dprintf(2, "CMD \n");
	// else if (new->type == PIPE)
	// 	dprintf(2, "PIPE \n");
	// else if (new->type == AND_IF)
	// 	dprintf(2, "AND_IF \n");
	// else if (new->type == OR_IF)
	// 	dprintf(2, "OR_IF \n");
	// else if (new->type == LESS)
	// 	dprintf(2, "LESS \n");
	// else if (new->type == GREAT)
	// 	dprintf(2, "GREAT \n");
	// else if (new->type == DLESS)
	// 	dprintf(2, "DLESS \n");
	// else if (new->type == DGREAT)
	// 	dprintf(2, "DGREAT \n");
	// else if (new->type == NEWLINE)
	// 	dprintf(2, "NEWLINE \n");
	// else 
// 	// 	dprintf(2, "UNKNOW \n");
	
// }

/*
 * redirs_new:
 * Cree une nouvelle structure t_redirs du type indique par le current token
 * Le nom du fichier vers lequel la redir se fait est indique par le next token
 * La next redir est initialisee a NULL
 */

t_redirs	*redirs_new(t_ms *ms, t_token *token, int type)
{
	t_redirs	*d;
	// t_redirtype	*rt;
	
	// rt = redirtype_new(ms, token_to_node(type));
	// if (!rt)
	// 	return (NULL);
	d = NULL;
	d = ft_calloc(1, sizeof(t_redirs));
	if (!d)
	{
		ms->exit_code = 255;
		// free(rt);
		return (NULL);
	}
	// rt_addback(&d->redirtype, rt);
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
	// if (d->type == CMD)
	// 	dprintf(2, "CMD \n");
	// else if (d->type == PIPE)
	// 	dprintf(2, "PIPE \n");
	// else if (d->type == AND_IF)
	// 	dprintf(2, "AND_IF \n");
	// else if (d->type == OR_IF)
	// 	dprintf(2, "OR_IF \n");
	// else if (d->type == LESS)
	// 	dprintf(2, "LESS \n");
	// else if (d->type == GREAT)
	// 	dprintf(2, "GREAT \n");
	// else if (d->type == DLESS)
	// 	dprintf(2, "DLESS \n");
	// else if (d->type == DGREAT)
	// 	dprintf(2, "DGREAT \n");
	// else if (d->type == NEWLINE)
	// 	dprintf(2, "NEWLINE \n");
	// else 
	// 	dprintf(2, "UNKNOW \n");
	// dprintf(2, "%s\n", d->filename);
	return (d);
}

/*
 * is_redir:
 * Verifie que le type envoye est un token redirection
 */

// int	is_redir(int type)
// {
// 	if (type == T_LESS || type == T_GREAT
// 		|| type == T_DLESS || type == T_DGREAT)
// 		return (1);
// 	return (0);
// }

/*
 * handle_red:
 * Cree une nouvelle structure t_redirs qui contient le nom du fichier correspondant a cette redirection. Cette structure est ajoutee a la liste des redirections de l'AST
 */

t_redirs	*handle_red(t_ms *ms, t_ast *new_ast)
{
//	dprintf(2, "REDIR\n");
//	dprintf(2, "%s\n", tok_to_str(ms->cur_tok));
	t_redirs	*new_redir;

	new_redir = NULL;
	if (ms->cur_tok->next_token->type != T_WORD)
		return (NULL);
	new_redir = redirs_new(ms, ms->cur_tok->next_token, ms->cur_tok->type);
	if (!new_redir)
		return (NULL);
	if (ms->cur_tok->next_token->dol)
		ft_doladd_back(&new_redir->dol, ms->cur_tok->next_token->dol);
	if (ms->cur_tok->next_token->wil)
		ft_wiladd_back(&new_ast->wil, ms->cur_tok->next_token->wil);
	redirs_add_back(&new_ast->redirs, new_redir);
	eat_token(ms, ms->cur_tok->type);
	eat_token(ms, ms->cur_tok->type);
	return (new_ast->redirs);
}
