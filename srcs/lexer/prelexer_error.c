/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prelexer_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:19:15 by mapoirie          #+#    #+#             */
/*   Updated: 2024/02/01 17:52:36 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../libft/libft.h"

int	check_quotes(char *str)
{
	int		qflag;
	int		i;
	char	c;

	qflag = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] && (str[i] == '\'' || str[i] == '\"'))
		{
			qflag = 1;
			c = str[i];
			i++;
			while (str[i] && str[i] != c)
				i++;
			if (str[i] && str[i] == c)
				qflag = 0;
			else if (!str[i])
				break ;
		}
	}
	return (qflag);
}

int	check_error_prelexer(t_ms *ms)
{
	if (check_quotes(ms->line) != 0)
	{
		free(ms->line);
		ms->exit_code = 2;
		return (ft_putstr_fd("minishell: quotes error\n", 2), 1);
	}
	return (0);
}
