#ifndef SIGNALS_H
# define SIGNALS_H

# define CMD_NOT_FOUND 127

# include "lexer.h"
# include "struct.h"

//----------------------- signals.c ------------------------
void	ignore_sigquit(void);
void	signal_newprompt(int signal);
void	preprompt_signals(void);
void	signal_newline(int signal);
void	postprompt_signals(void);

#endif
