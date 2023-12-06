/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 10:27:31 by ccarnot           #+#    #+#             */
/*   Updated: 2023/12/06 15:43:05 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/export.h"
#include "../libft/libft.h"

int	count_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (0);
	while (args[i])
		i++;
	return (i);
}

void	free_exit(t_ms *ms)
{
	if (ms->root)
	{
		free_root_ast(ms->root);
		ms->root = NULL;
	}
	if (ms->lexer)
	{
		token_lst_free(&ms->lexer->token_lst);
		free(ms->lexer);
		ms->lexer = NULL;
	}
	if (ms->env)
		ft_lstfree(&ms->env);
	if (ms->exp)
		ft_lstfree(&ms->exp);
	if (ms->wkdir)
		free(ms->wkdir);
	if (ms->old_wkdir)
		free(ms->old_wkdir);
	if (ms->line)
	{
		free(ms->line);
		ms->line = NULL;
	}
	if (ms)
		free(ms);
}

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

bool	is_notvalid(char c, int *error)
{
	if (c)
	{
		*error = 1;
		return (true);
	}
	return (false);
}

long long	atoll_exit(char *args, int *error)
{
	long long	n;
	int			sign;
	int			i;

	n = 0;
	sign = 1;
	i = 0;
	if (!args)
		return (0);
	while (args[i] && is_whitespace(args[i]))
		i++;
	if (args[i] && (args[i] == '+' || args[i] == '-'))
	{
		if (args[i] == '-')
			sign *= (-1);
		i++;
	}
	while (args[i] && args[i] >= '0' && args[i] <= '9')
	{
		n = n * 10 + args[i] - '0';
		if (is_toobig(n))
			return (2);
		i++;
	}
	while (args[i] && is_whitespace(args[i]))
		i++;
	if (is_notvalid(args[i], error))
		return (2);
	return (n * sign);
}

char	*join_strs(char *s1, char *s2)
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
	msg = join_strs(msg, cmd);
	if (!msg)
		free_minishell(ms, 1);
	if (details)
	{
		msg = join_strs(msg, details);
		if (!msg)
			free_minishell(ms, 1);
	}
	msg = join_strs(msg, error);
	if (!msg)
		free_minishell(ms, 1);
	ft_putstr_fd(msg, 2);
	free(msg);
	msg = NULL;
}

int	exec_exit(t_ms * ms, t_cmd *cmd)
{
	long long	exit code;
	int			error;

	error = 0;
	exit_code = atoll_exit(cmd->args, &error);
	if (count_args(cmd->args) == 1)
	{
		ft_putstr_fd("exit\n", 2);
		free_exit(ms)
		exit(ms->exit_code);
	}
	if (exit_code == 2 && error == 1)
	{
		ft_putstr_fd("exit\n", 2);
		exit_msg(ms, "exit", cmd->args[1], "numeric argument required\n");
		free_exit(ms)
		exit(exit_code);
	}
	if (count_args(cmd->args) > 2)
	{
		ft_putstr_fd("exit\n", 2);
		exit_msg(ms, "exit", "too many arguments");
		return (1); //A VERIF
	}
}
