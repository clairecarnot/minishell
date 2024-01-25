/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 10:27:31 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/25 18:11:45 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../libft/libft.h"

bool	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (true);
	return (false);
}

bool	is_toobig(unsigned long long n, int sign, int *error)
{
	if ((sign > 0 && n > LLONG_MAX)
		|| (sign < 0 && n > -(unsigned long long)(LLONG_MIN)))
	{
		*error = 1;
		return (true);
	}
	return (false);
}

bool	is_notnumeric(char *args, int *error)
{
	int			i;

	i = 0;
	if (!args)
		return (false);
	while (args[i] && is_whitespace(args[i]))
		i++;
	if (args[i] && (args[i] == '+' || args[i] == '-'))
		i++;
	while (args[i] && args[i] >= '0' && args[i] <= '9')
		i++;
	while (args[i] && is_whitespace(args[i]))
		i++;
	if (args[i])
	{
		*error = 1;
		return (true);
	}
	return (false);
}

void	exit_msg(t_ms *ms, char *cmd, char *details, char *error)
{
	(void)ms;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (details)
	{
		ft_putstr_fd(details, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(error, 2);
}
