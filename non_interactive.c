#include "./include/general.h"

void	non_interactive_mode_bis(t_ms *minishell)
{
	t_token	*tmp;

	if (*minishell->line == '\"' || *minishell->line == '\'')
		minishell->flag_q = 1;
	if (!lexer(minishell, minishell->line))
	{
		if (!minishell->lexer)
			free_minishell(minishell, 1); //ms exit code?
//		minishell->cur_tok = minishell->lexer->token_lst;
		tmp = minishell->cur_tok;
		if (parse(minishell) == -1)
		{
			minishell->cur_tok = tmp;
			get_next_line(0, 1);
			if (minishell->exit_code == 255)
				free_minishell(minishell, 255);
			else if (minishell->exit_code == 2)
				free_minishell(minishell, 2);
		}
		else
		{
			minishell->cur_tok = tmp;
			(pre_exec(minishell), get_next_line(0, 1));
			(free_exit(minishell), exit(minishell->exit_code));
		}
	}
}

int	non_interactive_mode(t_ms *minishell, char **env)
{
	char	*line;

	line = get_next_line(STDIN_FILENO, 0);
	if (!line)
		(get_next_line(0, 1), exit(0));
	line[ft_strlen(line) - 1] = '\0';
	minishell = init_ms(env);
	minishell->line = line;
	minishell->previous_exit_code = minishell->exit_code; //A CHECKER
	if (!check_error_prelexer(minishell->line))
		non_interactive_mode_bis(minishell);
	(get_next_line(0, 1), exit(1));
}

/*
 * Explication ft_isatty() (cas SIGPIPE)
 * ex: pour ./minishell(1) | ./minishell(2)
 * Le minishell(2) n'est pas un terminal, il va exit
 * Pour le minishell(1):
 * Le pipeline va remplacer la STDOUT par un pipe[1] 
 *  => comme le ms(2) va exit, il y aura un SIGPIPE si
 *  le ms(1) essaye d'ecrire dans le pipe[1]
 * On doit donc re-rediriger la sortie pipe[1] vers STDOUT
 *  afin d'eviter le SIGPIPE
 * Comme STDOUT est deja remplace par pipe[1], on utilise
 *  STDIN pour rediriger pipe[1] (effectivement, stdin et stdout
 *  se rapportent au meme fichier, le terminal donc bien que
 *   differents on peut remplacer l'un par l'autre)
 */

void	ft_isatty(t_ms *minishell, char **env)
{
	int		fd;

	if (!isatty(0))
		non_interactive_mode(minishell, env);
	if (isatty(0))
	{
		fd = open("/dev/stdin", O_RDWR);
		if (fd < 0)
			(perror("open failed"), exit(errno));
		if (dup2(fd, STDOUT_FILENO) == -1)
			(perror("dup2 failed"), close_if(&fd), exit(errno));
		close_if(&fd);
	}
}
