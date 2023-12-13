#include "../../../include/builtins.h"
#include "../libft/libft.h"

int	handle_echo_n(t_ms *ms, char **args, int *newline, int *after_n)
{
	(void)ms;
	int	i;
	int	j;

	i = 0;
	while (args[i])
	{
		j = 2;
		while (args[i][j] && args[i][j] == 'n')
			j++;
		if (args[i][j])
			break ;
		else
		{
			*newline = 0;
			*after_n = *after_n + 1;
		}
		i++;
	}
	return (0);
}

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
	i = 1;
	while (cmd->args[i])
	{
		if (after_n)
			after_n--;
		else
		{
			ft_putstr_fd(cmd->args[i], 1);
			if (cmd->args[i + 1])
				write(1, " ", 1);
		}
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}
