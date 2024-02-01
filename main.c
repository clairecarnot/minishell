/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:33:59 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:59:27 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/general.h"

t_ms	*init_ms_bis(t_ms *minishell, char **env)
{
	if (init_env(minishell, env))
		free_minishell(minishell, 255);
	init_workdir(minishell, 0);
	if (init_exp(minishell))
		free_minishell(minishell, 255);
	return (minishell);
}

t_ms	*init_ms(char **env)
{
	t_ms	*minishell;

	minishell = ft_calloc(1, sizeof(t_ms));
	if (!minishell)
		exit(255);
	minishell->lexer = NULL;
	minishell->cur_tok = NULL;
	minishell->root = NULL;
	minishell->env = NULL;
	minishell->exp = NULL;
	minishell->wkdir = NULL;
	minishell->old_wkdir = NULL;
	minishell->cur_node = NULL;
	minishell->pidlst = NULL;
	minishell->exit_code = 0;
	minishell->previous_exit_code = 0;
	minishell->in = -1;
	minishell->out = -1;
	minishell->hdlst = NULL;
	minishell->i = 0;
	minishell->j = 0;
	minishell->i_w = 0;
	minishell->value = NULL;
	return (init_ms_bis(minishell, env));
}

char	*display_prompt(t_ms *ms)
{
	char	*line;
	int		exit_code;

	line = NULL;
	exit_code = ms->exit_code;
	line = readline("minishell$ ");
	if (!line)
	{
		if (ms->env)
			ft_lstfree(&ms->env);
		if (ms->exp)
			ft_lstfree(&ms->exp);
		if (ms->wkdir)
			free(ms->wkdir);
		if (ms->old_wkdir)
			free(ms->old_wkdir);
		if (ms->home)
			free(ms->home);
		free(ms);
		ft_putstr_fd("exit\n", 1);
		exit(exit_code);
	}
	if (line[0] != '\0')
		add_history(line);
	return (line);
}

void	main_bis(t_ms *minishell)
{
	t_token	*tmp;

	if (*minishell->line == '\"' || *minishell->line == '\'')
		minishell->flag_q = 1;
	if (!lexer(minishell, minishell->line))
	{
		if (!minishell->lexer)
			free_minishell(minishell, 1);
		tmp = minishell->cur_tok;
		if (parse(minishell) == -1)
		{
			minishell->cur_tok = tmp;
			if (minishell->exit_code == 255)
				free_minishell(minishell, 255);
			else if (minishell->exit_code == 2)
				free_minishell(minishell, 0);
		}
		else
		{
			minishell->cur_tok = tmp;
			pre_exec(minishell);
			free_minishell(minishell, 0);
		}
	}
}

int	main(int argc, char **argv, char **env)
{
	t_ms	*minishell;

	minishell = NULL;
	((void)argc, (void)argv, ft_isatty(minishell, env));
	minishell = init_ms(env);
	while (1)
	{
		preprompt_signals();
		minishell->line = display_prompt(minishell);
		if (g_exit_code == 2)
		{
			minishell->exit_code = 130;
			g_exit_code = 0;
		}
		if (minishell->line[0] != '\0')
		{
			minishell->previous_exit_code = minishell->exit_code;
			minishell->exit_code = 0;
		}
		if (!check_error_prelexer(minishell->line))
			main_bis(minishell);
		init_workdir(minishell, 1);
	}
	free_minishell(minishell, 1);
	return (0);
}
