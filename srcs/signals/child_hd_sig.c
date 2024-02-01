/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_hd_sig.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:31:30 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:31:31 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/signals.h"

void	sig_hdoc(int signal)
{
	(void)signal;
	close(0);
	ft_putstr_fd("\n", 2);
	g_exit_code = 2;
}

void	hd_signals(void)
{
	signal(SIGINT, sig_hdoc);
	signal(SIGQUIT, SIG_IGN);
}

/*
 * child_signals
 * Called in a child children just after a fork, to reset the signals
 * Resets the default actions for SIGINT AND SIGQUIT
 * Once a garbage manager has been created, the function is called
 * again (bis) to handle SIGPIPE (free)
 */

void	child_sigpipe(int signal)
{
	if (signal == SIGPIPE)
	{
		go_garbage(1, NULL);
		exit(0);
	}
}

void	child_signals_bis(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, child_sigpipe);
}

void	child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
