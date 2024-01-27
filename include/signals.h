#ifndef SIGNALS_H
# define SIGNALS_H

# include "general.h"
extern int      g_exit_code;

//----------------------- parent_sig.c ------------------------
void	signal_newprompt(int signal);
void	preprompt_signals(void);
void	postfork_ms_sig(int signal);
void	ms_signals(void);

//----------------------- child_hd_sig.c ------------------------
void	sig_hdoc(int signal);
void	hd_signals(void);
void	child_sigpipe(int signal);
void	child_signals_bis(void);
void	child_signals(void);

#endif
