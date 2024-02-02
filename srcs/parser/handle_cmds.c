/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:40:19 by mapoirie          #+#    #+#             */
/*   Updated: 2024/02/02 09:46:33 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

t_dol	*dup_tokdol(t_ms *ms, t_dol *tok_dol)
{
	t_dol	*new;

	new = ft_calloc(1, sizeof(t_dol));
	if (!new)
		return (ms->exit_code = 255, NULL);
	new->d = lst_dup_int(tok_dol->d);
	if (!new->d)
		return (free(new), ms->exit_code = 255, NULL);
	new->c = lst_dup_int(tok_dol->c);
	if (!new->c)
		return (ft_lstfree(&new->d), free(new), ms->exit_code = 255, NULL);
	return (new);
}

int	ft_doladd_back(t_ms *ms, t_dol **dol, t_dol *tok_dol)
{
	t_dol	*new;
	t_list	*ptr_c;
	t_list	*ptr_d;

	new = dup_tokdol(ms, tok_dol);
	if (!new)
		return (ms->exit_code = 255, 1);
	if (*dol == NULL)
		*dol = new;
	else
	{
		ptr_d = (*dol)->d;
		ptr_c = (*dol)->c;
		while (ptr_d->next)
			ptr_d = ptr_d->next;
		ptr_d->next = new->d;
		while (ptr_c->next)
			ptr_c = ptr_c->next;
		ptr_c->next = new->c;
		free(new);
	}
	return (0);
}

int	ft_wiladd_back(t_ms *ms, t_wil **wil, t_wil *tok_wil)
{
	t_wil	*new;
	t_list	*ptr_w;

	new = ft_calloc(1, sizeof(t_wil));
	if (!new)
		return (ms->exit_code = 255, 1);
	new->w = lst_dup_int(tok_wil->w);
	if (!new->w)
		return (free(new), ms->exit_code = 255, 1);
	if (*wil == NULL)
		*wil = new;
	else
	{
		ptr_w = (*wil)->w;
		while (ptr_w->next)
			ptr_w = ptr_w->next;
		ptr_w->next = new->w;
		free(new);
	}
	return (0);
}

/*
 * add_cmd_args:
 * Cree une nouvelle structure t_list qui a pour content la
 *  valeur de la commande ou de l'argument qu'on ajoute a la 
 *  liste d'arguments de l'AST
 */

t_list	*add_cmd_args(t_ms *ms, t_ast *new_ast)
{
	char	*cmd;
	t_list	*new_arg;

	cmd = NULL;
	if (ms->cur_tok->value)
	{
		cmd = ft_strdup(ms->cur_tok->value);
		if (!cmd)
			return (ms->exit_code = 255, NULL);
	}
	new_arg = ft_lstnew(cmd);
	if (!new_arg)
		return (free_if(cmd), ms->exit_code = 255, NULL);
	ft_lstadd_back(&new_ast->args, new_arg);
	if (ms->cur_tok->dol)
	{
		if (ft_doladd_back(ms, &new_ast->dol, ms->cur_tok->dol))
			return (ms->exit_code = 255, NULL);
	}
	if (ms->cur_tok->wil)
	{
		if (ft_wiladd_back(ms, &new_ast->wil, ms->cur_tok->wil))
			return (ms->exit_code = 255, NULL);
	}
	return (eat_token(ms, T_WORD), new_ast->args);
}
