#include "../../include/exec.h" 

void	redefine_datapos(char **new_args, int i, int j, int data[5])
{
	data[3] = 1;
	data[4] = data[2] - (ft_strlen(new_args[i]) - (j - 1));
}

/*
 * redefine_args_bis
 * See below - copies into a new char** the original cmd->arg up to the i-1 arg
 * [here i = 0 so the first part does not matter]
 * the new cmd->args[i] is the new arg d[0]
 * the new cmd->args[i+1] is the new arg d[1]
 * The rest of cmd->args correspond to the original cmd->arg
 *
 * If return = NULL, there was a bad malloc
 */

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
	while (k < size + 1)
	{
		new_args[k] = ft_strdup(cmd->args[k - 1]);
		if (!new_args[k])
			return (free_tab(new_args), NULL);
		k++;
	}
	new_args[k] = NULL;
	return (new_args);
}

/*
 * redefine_args:
 * See below - corresponds to the second use,
 * ex : ech$a where a="o bonjour"
 * The function calculates the size of the original cmd->args
 * The new cmd->args[0] (d[0] = "echo") corresponds to all chars
 *  before the first space so if the spc is at char 0 (j = 0),
 *  the arguments won't be modified (cmd non valid)
 *  ex : ech$a where a="   o bonjour"
 * Then a second d[1]="bonjour" corresponds to everything that
 *  comes after the first space
 * The new cmd->args is redefined through a new char** of size+1 args;
 * cmd->args[i] and cmd->args[i+1] correspond to the new args defined above
 * data positions are updated so that we pass from cmd->arg[i]
 *  to cmd->arg[i+1] and j correspond to the char after the latest
 *  expanded char
 *
 * If return = NULL, there was a bad malloc
 */

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
		return (free_tab(cmd->args), NULL);
	ft_strlcpy(d[0], cmd->args[i], j + 1);
	d[1] = ft_calloc(ft_strlen(&cmd->args[i][j + 1]) + 1, sizeof(char));
	if (!d[1])
		return (free_tab(cmd->args), free(d[0]), NULL);
	ft_strlcpy(d[1], &cmd->args[i][j + 1], ft_strlen(&cmd->args[i][j + 1]) + 1);
	new_args = ft_calloc(size + 2, sizeof(char *));
	if (!new_args)
		return (free_tab(cmd->args), free(d[0]), free(d[1]), NULL);
	new_args = redefine_args_bis(cmd, d, new_args, i);
	if (!new_args)
		return (free_tab(cmd->args), NULL);
	redefine_datapos(new_args, i, j, data);
	return (free_tab(cmd->args), new_args);
}

/*
 * args_redef:
 * 	First use, it checks if the argument cmd->args[i] that was expanded
 * in the previous function is != NULL, otherwise it returns 1 (error)
 *
 *	Second use, in case i = 0, meaning we are in cmd->arg[0],
 * the function goes to redefine_args to check the result of the expand. 
 * Indeed, in case the expand result contains new spaces, cmd->args[0] 
 * should be split in more arguments, meaning cmd->args has to be redefined
 * ex: ech$a where a="o bonjour" 
 * 	needs to be split into cmd->args[0]="echo" and cmd->args[1]="bonjour"
 * 	(we add one argument)
 *
 *	If return = 1, it means there was a bad malloc
 */

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
