#include "../../include/exec.h" 

char	**redefine_args_bis(t_cmd *cmd, char *d[2], char **new_args, int i)
{
	int		size;
	int		k;

	size = tab_size(cmd->args);
	k = 0;
	while (k < i)
	{
		new_args[k] = ft_strdup(cmd->args[k]);
		if (!new_args[k])
			return (free_tab(new_args), free(d[0]), free(d[1]), NULL);
		k++;
	}
	new_args[k++] = d[0];
	new_args[k++] = d[1];
	while (k < size)
	{
		new_args[k] = ft_strdup(cmd->args[k - 1]);
		if (!new_args[k])
			return (free_tab(new_args), NULL);
		k++;
	}
	new_args[k] = NULL;
	return (new_args);
}

char	**redefine_args(t_cmd *cmd, int i, int j, int data[5])
{
	char	**new_args;
	char	*d[2];
	int		size;

	size = tab_size(cmd->args);
	while (cmd->args[i][j] && cmd->args[i][j] != ' ')
		j++;
	if (j == 0)
		return (cmd->args);
	d[0] = ft_calloc(j + 2, sizeof(char));
	if (!d[0])
		return (NULL);
	ft_strlcpy(d[0], cmd->args[i], j + 1);
	d[1] = ft_calloc(ft_strlen(&cmd->args[i][j + 1]) + 1, sizeof(char));
	if (!d[1])
		return (free(d[0]), NULL);
	ft_strlcpy(d[1], &cmd->args[i][j + 1], ft_strlen(&cmd->args[i][j + 1]) + 1);
	new_args = ft_calloc(size + 2, sizeof(char *));
	if (!new_args)
		return (free(d[0]), free(d[1]), NULL);
	new_args = redefine_args_bis(cmd, d, new_args, i);
	data[3] = 1;
	data[4] = data[2] - (ft_strlen(new_args[i]) - (j - 1));
	return (free_tab(cmd->args), new_args);
}

int	args_redef(t_cmd *cmd, int i, int j, int data[5])
{
	if (!cmd->args[i])
		return (1);
	if (i == 0 && contains_spc(cmd->args[i], j, data))
		cmd->args = redefine_args(cmd, i, j, data);
	if (!cmd->args)
		return (1);
	return (0);
}
