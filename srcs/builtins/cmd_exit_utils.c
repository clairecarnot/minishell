/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 10:27:31 by ccarnot           #+#    #+#             */
/*   Updated: 2023/12/06 18:09:22 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"
#include "../libft/libft.h"

bool	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (true);
	return (false);
}

bool	is_toobig(long long n, int *error)
{
	if (n > LLONG_MAX || n < LLONG_MIN)
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

char	*join_strs(t_ms *ms, char *s1, char *s2)
{
	char	*dest;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	dest = malloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 3);
	if (!dest)
	{
		free(s1);
		ms->exit_code = 134;
		return (NULL);
	}
	i = -1;
	while (s1[++i])
		dest[i] = s1[i];
	dest[i++] = ':';
	dest[i++] = ' ';
	j = -1;
	while (s2[++j])
		dest[i + j] = s2[j];
	dest[i + j] = '\0';
	free(s1);
	return (dest);
}

void	exit_msg(t_ms *ms, char *cmd, char *details, char *error)
{
	char	*msg;

	msg = ft_strdup("minishell: ");
	if (!msg)
		free_minishell(ms, 1);
	msg = join_strs(ms, msg, cmd);
	if (!msg)
		free_minishell(ms, 1);
	if (details)
	{
		msg = join_strs(ms, msg, details);
		if (!msg)
			free_minishell(ms, 1);
	}
	msg = join_strs(ms, msg, error);
	if (!msg)
		free_minishell(ms, 1);
	ft_putstr_fd(msg, 2);
	free(msg);
	msg = NULL;
}
