/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_sig.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:31:35 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:31:48 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/signals.h"

int	g_exit_code = 0;

/*
 * ms_signals & postfork_ms_sig
 * Called in the parent just after a fork, to reset the signals
 * Action for SIGQUIT is to do nothing, 
 * and action for SIGINT is to print a newline
 */

void	postfork_ms_sig(int signal)
{
	(void)signal;
}

void	ms_signals(void)
{
	signal(SIGINT, postfork_ms_sig);
	signal(SIGQUIT, postfork_ms_sig);
}

/*
 * preprompt_signals & signal_new_prompt
 * Called when the shell expects an interaction (user input)
 *  and before children are forked
 *
 * SIGQUIT: is ignored
 *
 * SIGINT:
 * print a newline
 * indicate to readline that we moved on a new empty line
 * replace the readline buffer with ""
 * redisplay the new buffer ("")
 *
 * exit_code indicates a SIGGINT was received
 */

void	signal_newprompt(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_exit_code = 2;
	}
}

void	preprompt_signals(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, signal_newprompt);
}
