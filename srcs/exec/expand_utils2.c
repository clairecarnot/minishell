#include "../../include/exec.h" 

int	delimitate_var(char *arg, int i, t_dol **dol)
{
	int	j;

	j = i + 1;
	while (arg[j] && arg[j] != '$' && arg[j] != '\"' && arg[j] != '\''
		&& (*dol)->c->n--)
		j++;
	return (j);
}

void	free_if(char *str)
{
	if (str)
		free(str);
}

int	tab_size(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

int	contains_spc(char *arg, int j, int data[5])
{
	int	i;

	i = 0;
	if (data[1])
		return (0);
	while (arg[j] && i < data[2])
	{
		if (arg[j] == ' ')
			return (1);
		j++;
		i++;
	}
	return (0);
}
