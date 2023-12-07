/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 10:27:31 by ccarnot           #+#    #+#             */
/*   Updated: 2023/12/06 18:47:09 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"
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
		free_root_ast(ms->root);
	if (ms->lexer)
	{
		token_lst_free(&ms->lexer->token_lst);
		free(ms->lexer);
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
		free(ms->line);
	if (ms)
		free(ms);
}

long long	atoll_exit(char *args, int *error)
{
	unsigned long long	n;
	int					sign;
	int					i;

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
			sign = -1;
		i++;
	}
	while (args[i] && args[i] >= '0' && args[i] <= '9')
	{
		n = n * 10 + args[i] - '0';
		if (is_toobig(n, sign, error))
			return (2);
		i++;
	}
	return (n * sign);
}

int	get_exit_code(char *args, int *error)
{
	long long	exit_code;

	if (is_notnumeric(args, error))
		return (2);
	exit_code = atoll_exit(args, error);
	if (exit_code == 2 && *error == 1)
		return (2);
	exit_code %= 256;
	return (exit_code);
}

int	exec_exit(t_ms *ms, t_cmd *cmd)
{
	int	exit_code;
	int	error;

	error = 0;
	if (isatty(0) == 1)
		ft_putstr_fd("exit\n", 1);
	if (count_args(cmd->args) == 1)
		exit_code = ms->exit_code;
	else
	{
		exit_code = get_exit_code(cmd->args[1], &error);
		if (exit_code == 2 && error == 1)
			exit_msg(ms, "exit", cmd->args[1], "numeric argument required\n");
		else if (count_args(cmd->args) > 2)
			return (exit_msg(ms, "exit", NULL, "too many arguments\n"), 1);
	}
	(free_exit(ms), exit(exit_code));
	return (0);
}
