 #include "../../include/exec.h" 

char	*get_varvalue(char *var)
{
	char	*value;

	value = NULL;
	value = getenv(var);
	free(var);
	return (value);
}


char	*skip_dol(char *arg, int i, int j)
{
	char	*newarg;
	int		k;

	k = 0;
	newarg = NULL;
	newarg = ft_calloc(ft_strlen(arg) - (j - i) + 1, sizeof(char));
	if (!newarg)
		return (NULL);
	while (k < i)
	{
		newarg[k] = arg[k];
		k++;
	}
	while (arg[j])
	{
		newarg[k] = arg[j];
		j++;
		k++;
	}
	new_arg[k] = '\0';
	return (new_arg);
}

char	*repl_dol(char *arg, char *var, int i, int j)
{
	char	*newarg;
	int		k;

	k = 0;
	newarg = NULL;
	newarg = ft_calloc(ft_strlen(arg) - (j - i) + ft_strlen(var) + 1, sizeof(char));
	if (!newarg)
		return (NULL);
	while (k < i)
	{
		newarg[k] = arg[k];
		k++;
	}
	while (*var)
	{
		newarg[k] = *var;
		var++;
	}
	while (arg[j])
	{
		newarg[k] = arg[j];
		j++;
		k++;
	}
	new_arg[k] = '\0';
	return (new_arg);
}

char	*expand_dol(char *arg)
{
	char	*exp_arg;
	int		i;
	char	*var;

	exp_arg = NULL;
	var = NULL;
	i = 0;
	while (arg[i] && arg[i] != '$')
		i++;
	j = i + 1;
	while (arg[j] && arg[j] != '$' && arg[j] != '\"' && arg[j] != '\'')
	{
		j++;
		var = ft_calloc(j - i + 1, sizeof(char));
		if (!var)
			return (free(arg), NULL);
		ft_strlcpy(var, &arg[i + 1], j - (i + 1));
		var = get_varvalue(var);
	}
	if (!var)
		exp_arg = skip_dol(arg, i, j);
	else
		exp_arg = repl_dol(arg, var, i, j);
	if (!exp_arg)
		return (free(arg), NULL);
	free(arg);
	return (exp_arg);
}

int	cmd_expand(char **args, t_list *dol)
{
	int	i;
	int	j;

	if (!args)
		return (0);
	i = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] == '$')
			{
				if (dol->n == 1)
				{
					args[i] = expand_dol(args[i]);
					if (!args[i])
						return (1);
				}
				dol = dol->next;
			}
			j++;
		}
		i++;
	}
	return (0);
}
