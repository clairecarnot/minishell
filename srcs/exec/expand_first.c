#include "../../include/exec.h" 

char	*ft_trimvar_first(char *value)
{
//	dprintf(2, "trim var first\n");
//	dprintf(2, "value = %s\n", value);
//	char	*trim_val;
	char	*new_val;
	int	i;
	int	j;

	i = 0;
	j = 0;
//	trim_val = NULL;
//	dprintf(2, "count = %lu\n", ft_strlen(value) - count_consec_spc(value) + 1);
	new_val = ft_calloc(ft_strlen(value) - count_consec_spc(value) + 1,
			sizeof(char));
	if (!new_val)
		return (NULL);
	while (value[i])
	{		
//		dprintf(2, "value[i] = %c", value[i]);
//		dprintf(2, "-\n");
		new_val[j] = value[i];
		if (value[i] == ' ')
		{
			while (value[i] && value[i] == ' ')
				i++;
			j++;
		}
		else
		{
			i++;
			j++;
		}
	}
	new_val[j] = '\0';
//	dprintf(2, "new_val = %s\n", new_val);
//	trim_val = trim_beg(new_val, " ");
//	if (!trim_val)
//		return (free(new_val), NULL);
//	dprintf(2, "trim_val = %s\n", trim_val);
//	dprintf(2, "new_val = %s\n", new_val);
	return (new_val);
}

char	*get_varvalue_first(t_ms *ms, char *arg, int i, int j)
{
//	dprintf(2, "var value first\n");
	char	*var;
	char	*value;
	char	*new_var;

	value = NULL;
	if (arg[i + 1] == '?')
		return (exp_exitcode(ms));
	var = ft_calloc(j - i, sizeof(char));
	if (!var)
	{
		ms->exit_code = 255;
		return (NULL);
	}
	ft_strlcpy(var, &arg[i + 1], j - i);
	value = ft_getenv(ms, var);
	if (!value)
		return (free(var), NULL);
	new_var = ft_trimvar_first(value);
	if (!new_var)
	{
		ms->exit_code = 255;
		return (free(value), free(var), NULL);
	}
	return (free(value), free(var), new_var);
}

char	*expfirst(t_ms *ms, char *arg, int data[5], t_dol **dol)
{
//	dprintf(2, "expand dol first\n");
//	dprintf(2, "expand dol1\n");
//	dprintf(2, "arg = %s\n", arg);
//	dprintf(2, "dol_nb =  %d\n", data[0]);
	int		i;
	int		j;
	char	*exp_arg;
	char	*var;
	int		dol_nb;

	dol_nb = data[0];
	exp_arg = NULL;
	var = NULL;
	i = 0;
	while (arg[i] && (arg[i] != '$' || (arg[i] == '$' && dol_nb > 0)))
	{
		if (arg[i] == '$')
			dol_nb--;
		i++;
	}
//	dprintf(2, "i = %d\n", i);
//	dprintf(2, "expand dol2\n");
//	dprintf(2, "%c\n", arg[i]);
	if (!arg[i] || !arg[i + 1] || dol_standalone(&arg[i + 1], dol))
//	{
//		dprintf(2, "ici\n");
		return (arg);
//	}
//	dprintf(2, "%s\n", arg);
//	dprintf(2, "arg[%d] = %c\n", i, arg[i]);
	if (arg[i + 1] == '$')
		arg = keep_one_dol_only(ms, arg, i, dol);
//	dprintf(2, "expand dol2 - 2\n");
	if (!arg)
		return (NULL);
//	dprintf(2, "arg[%d] = %c\n", i, arg[i]);
//	dprintf(2, "expand dol3\n");
//	dprintf(2, "%s\n", arg);
	j = i + 1;
	while (arg[j] && arg[j] != '$' && arg[j] != '\"' && arg[j] != '\''
			&& (*dol)->c->n--)
		j++;
	var = get_varvalue_first(ms, arg, i, j);
//	dprintf(2, "var = %s\n", var);
	if (!var && ms->exit_code == 255)
		return (free(arg), NULL);
	else if (!var)
		exp_arg = skip_dol(arg, i, j, data);
	else
		exp_arg = repl_dol(arg, var, i, j);
//	dprintf(2, "expand dol4\n");
//	dprintf(2, "%s\n", exp_arg);
	if (!exp_arg)
	{
		ms->exit_code = 255;
		if (var)
			free(var);
		return (free(arg), NULL);
	}
	if (var)
	{
		data[2] += ft_strlen(var);
//		dprintf(2, "data[2] = %d\n", data[2]);
		free(var);
	}
	data[0] -= 1;
//	dprintf(2, "exp_arg = %s\n", exp_arg);
	return (free(arg), exp_arg);
}
