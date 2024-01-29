/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 10:27:31 by ccarnot           #+#    #+#             */
/*   Updated: 2024/01/29 18:12:08 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"

void	free_exit_bis(t_ms *ms)
{
	if (ms->wkdir)
		free(ms->wkdir);
	if (ms->old_wkdir)
		free(ms->old_wkdir);
	if (ms->home)
		free(ms->home);
	if (ms->line)
		free(ms->line);
	ms->line = NULL;
	if (ms->hdlst)
		ft_lstfree(&ms->hdlst);
	if (ms)
		free(ms);
}

void	free_exit(t_ms *ms)
{
	t_list		*hdtmp;

	close_if(&ms->in);
	close_if(&ms->out);
	hdtmp = ms->hdlst;
	while (hdtmp && isatty(0) && isatty(1))
	{
		if (hdtmp->content)
			unlink(hdtmp->content);
		hdtmp = hdtmp->next;
	}
	if (ms->pidlst)
		ft_intlstfree(&ms->pidlst);
	free_wil_dol(ms);
	if (ms->root)
		free_root_ast(ms->root);
	ms->root = NULL;
	if (ms->lexer)
		(token_lst_free(&ms->lexer->token_lst), free(ms->lexer));
	ms->lexer = NULL;
	if (ms->env)
		ft_lstfree(&ms->env);
	if (ms->exp)
		ft_lstfree(&ms->exp);
	free_exit_bis(ms);
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
	if (isatty(0) == 1 && isatty(1) == 1)
		ft_putstr_fd("exit\n", 1);
	if (tab_size(cmd->args) == 1)
		exit_code = ms->exit_code;
	else
	{
		exit_code = get_exit_code(cmd->args[1], &error);
		if (exit_code == 2 && error == 1)
			exit_msg(ms, "exit", cmd->args[1], "numeric argument required\n");
		else if (tab_size(cmd->args) > 2)
			return (exit_msg(ms, "exit", NULL, "too many arguments\n"), 1);
	}
	free_cmd(cmd);
	(free_exit(ms), exit(exit_code));
	return (0);
}
