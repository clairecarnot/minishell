/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:30:02 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:30:03 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

int	cmd_wildcard_free(t_wildcard *wildc)
{
	if (wildc->tmp_args)
		free_tab(wildc->tmp_args);
	return (free(wildc), 1);
}

int	wildcards_init(DIR **d, struct dirent **file)
{
	*d = opendir(".");
	if (!(*d))
	{
		perror("opendir");
		return (1);
	}
	*file = readdir(*d);
	if (!(*file))
	{
		perror("readdir");
		return (1);
	}
	return (0);
}
