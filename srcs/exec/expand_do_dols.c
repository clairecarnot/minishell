#include "../../include/exec.h" 

char	*skip_dol(char *arg, int i, int j, int data[5])
{
	char	*new_arg;
	int		k;

	k = 0;
	new_arg = NULL;
	new_arg = ft_calloc(ft_strlen(arg) - (j - i) + 1, sizeof(char));
	if (!new_arg)
		return (NULL);
	while (k < i)
	{
		new_arg[k] = arg[k];
		k++;
	}
	while (arg[j])
	{
		new_arg[k] = arg[j];
		j++;
		k++;
	}
	new_arg[k] = '\0';
	data[0] -= 1;
	data[1] = 1;
	return (new_arg);
}

char	*repl_dol(char *arg, char *var, int i, int j)
{
	char	*new_arg;
	int		k;

	k = -1;
	new_arg = NULL;
	new_arg = ft_calloc(ft_strlen(arg) - (j - i)
			+ ft_strlen(var) + 1, sizeof(char));
	if (!new_arg)
		return (NULL);
	while (++k < i)
		new_arg[k] = arg[k];
	while (*var)
	{
		new_arg[k] = *var;
		var++;
		k++;
	}
	while (arg[j])
	{
		new_arg[k] = arg[j];
		j++;
		k++;
	}
	new_arg[k] = '\0';
	return (new_arg);
}

char	*keep_one_dol_only(t_ms *ms, char *arg, int i, t_dol **dol)
{
	char	*new_arg;
	int		j;
	int		k;

	new_arg = NULL;
	j = i + 1;
	while (arg[j] && arg[j] == '$')
	{
		j++;
		(*dol)->d = (*dol)->d->next;
		(*dol)->c = (*dol)->c->next;
	}
	new_arg = ft_calloc(ft_strlen(arg) - (j - i) + 2, sizeof(char));
	if (!new_arg)
	{
		ms->exit_code = 255;
		return (free(arg), NULL);
	}
	k = -1;
	while (++k <= i)
		new_arg[k] = arg[k];
	while (arg[j])
		new_arg[k++] = arg[j++];
	new_arg[k] = '\0';
	return (free(arg), new_arg);
}

int	dol_standalone_return(char c)
{
	if (!c)
		return (1);
	else
		return (0);
}

int	dol_standalone(char *arg, t_dol **dol)
{
	int	i;

	i = 0;
	if (arg[i] && arg[i] == ' ')
	{
		while (arg[i] && arg[i] == ' ')
			i++;
		return (dol_standalone_return(arg[i]));
	}
	if (arg[i] && arg[i] == '$')
	{
		while (arg[i] && arg[i] == '$')
			i++;
		while (arg[i] && arg[i] == ' ')
			i++;
		return (dol_standalone_return(arg[i]));
	}
	if ((*dol)->c->n == 0)
		return (1);
	return (0);
}
