#include "../../../include/builtins.h"
#include "../libft/libft.h"

/*
 * handle_echo_n:
 * Counts the number of following '-n' options (as many n as wanted per option
 * but nothing else)
 * If there is at least one valid -n, a newline won't be printed
 */

int	handle_echo_n(t_ms *ms, char **args, int *newline, int *after_n)
{
	int	i;
	int	j;

	(void)ms;
	i = 0;
	while (args[i] && ft_strncmp(args[i], "-n", 2) == 0)
	{
		if (ft_strlen(args[i]) > 2)
		{
			j = 2;
			while (args[i][j] && args[i][j] == 'n')
				j++;
			if (args[i][j])
				break ;
		}
		*newline = 0;
		*after_n = *after_n + 1;
		i++;
	}
	return (0);
}

/*
 * exec_echo:
 * executes echo builtin
 * 	'newline' = 1 indicates a newline should be printed at the end; it depends
 * if there is one or several -n options (-n = no newline)
 *	'after_n' = x indicates the number of '-n' arguments to pass before reaching
 * the first word to be printed; a space is printed between non-empty words
 *
 * Both 'newline' and 'after_n' params are updated thanks to the handle_echo_n 
 * function
 */

int	exec_echo(t_ms *ms, t_cmd *cmd)
{
	int	newline;
	int	i;
	int	after_n;

	newline = 1;
	after_n = 0;
	if (cmd->args[1] && ft_strncmp(cmd->args[1], "-n", 2) == 0)
		if (handle_echo_n(ms, &cmd->args[1], &newline, &after_n))
			return (1);
	i = 0;
	while (cmd->args[++i])
	{
		if (after_n)
			after_n--;
		else
		{
			ft_putstr_fd(cmd->args[i], 1);
			if (cmd->args[i + 1] && ft_strlen(cmd->args[i + 1])
				&& (cmd->args[i - 1] && ft_strlen(cmd->args[i - 1])))
				write(STDOUT_FILENO, " ", 1);
		}
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
