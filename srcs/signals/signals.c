#include "../../include/builtins.h"
#include "../../include/signals.h"

int	g_exit_code = 0;

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
 * signal_new_prompt
 * Set the following actions when SIGINT is received:
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

/*
 * preprompt_signals
 * Called when the shell expects an interaction (user input)
 * Sets the actions for when the SIGQUIT (ctrl-c) and SIGINT (ctrl-\) signals
 * SIGINT: resets the prompt
 * SIGQUIT: is ignored
 */

void	preprompt_signals(void)
{
	signal(SIGQUIT, SIG_IGN); //A PROTEGER
	signal(SIGINT, signal_newprompt);
}

/*
 * child_signals
 * Called in a child children just after a fork, to reset the signals
 * Resets the default actions for SIGINT AND SIGQUIT
 */

void	child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/*
 * ms_signals & postfork_ms_sig
 * Called in the parent just after a fork, to reset the signals
 * Action for SIGQUIT is to do nothing, 
 * and action for SIGINT is to print a newline
 */

void	postfork_ms_sig(int signal)
{
	if (signal == SIGINT)
		ft_putstr_fd("\n", 2);
}

void	ms_signals(void)
{
//	signal(SIGINT, SIG_IGN);
	signal(SIGINT, postfork_ms_sig);
	signal(SIGQUIT, postfork_ms_sig);
}
