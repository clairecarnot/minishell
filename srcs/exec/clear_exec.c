#include "../../include/exec.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd->args)
		free_tab(cmd->args);
	if (cmd->bin_paths)
		free_tab(cmd->bin_paths);
	free(cmd);
}

int	close_if(int *fd)
{
	if (*fd > 0)
	{
		if (close(*fd) < 0)
			return (-1);
		*fd = -1;
	}
	return (0);
}