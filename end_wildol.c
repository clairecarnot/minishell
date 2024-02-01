/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_wildol.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:33:49 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:33:50 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/general.h"

void	wil_free(t_wil **wil)
{
	if (wil)
	{
		if ((*wil)->w)
			ft_lstfree(&(*wil)->w);
		if (*wil)
			free(*wil);
		*wil = NULL;
	}
}

void	dol_free(t_dol **dol)
{
	if (dol)
	{
		if ((*dol)->d)
			ft_lstfree(&(*dol)->d);
		if ((*dol)->c)
			ft_lstfree(&(*dol)->c);
		if (*dol)
			free(*dol);
		*dol = NULL;
	}
}

void	free_wil_dol(t_ms *ms)
{
	t_token	*tmp;

	tmp = ms->cur_tok;
	while (tmp)
	{
		if (tmp->dol)
			dol_free(&tmp->dol);
		if (tmp->wil)
			wil_free(&tmp->wil);
		tmp = tmp->next_token;
	}
}
