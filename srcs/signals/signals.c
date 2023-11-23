#include "../../include/signals.h"

int	g_exit_code = 0;

/*
 * ignore_sigquit
 * Set the following action when SIGQUIT is received:
 * ignore signal
 */

void	ignore_sigquit(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
}

/*
 * signal_new_prompt
 * Set the following actions when SIGQUIT is received:
 * output (print) a newline
 * indicate to readline that we moved on a new empty line
 * replace the readline buffer with ""
 * redisplay the new buffer ("")
 */

void	signal_newprompt(int signal)
{
	if (signal == SIGINT)
	{
		printf("\n");
		rl_on_newline();
		rl_replace_line("", 0);
		re_redisplay();
	}
}

/*
 * preprompt_signals
 * Called when the shell expects an interaction (user input)
 * Sets the actions for when the SIGQUIT (ctrl-c) and SIGINT (ctrl-\) signals
 * SIGINT: resets the prompt (=new blank line)
 * SIGQUIT: is ignored
 */

void	preprompt_signals(void)
{
	struct sigaction	act;

	ignore_sigquit();
	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_newprompt;
	sigaction(SIGINT, &act, NULL);
}

/*
 * signal_newline
 * Prints a newline
 */

void	signal_newline(int signal)
{
	(void)signal;
	rl_on_new_line();
}

/*
 * postprompt_signals
 * Called when the shell is non-interactive (already received user input) = it means that only the foreground=running processes need to react to the received signals (and not minishell)
 * Sets the actions for when the SIGQUIT (ctrl-c) and SIGINT (ctrl-\) signals
 * They simply print a newline
 */

void	postprompt_signals(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_newline;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
}
