#include "../../include/exec.h" 

char	*ft_trimvar_first(char *value)
{
	char	*v;
	int		i;
	int		j;

	i = 0;
	j = 0;
	v = ft_calloc(ft_strlen(value) - count_consec_spc(value) + 1, sizeof(char));
	if (!v)
		return (NULL);
	while (value[i])
	{		
		v[j] = value[i];
		if (value[i] == ' ')
		{
			while (value[i] && value[i] == ' ')
				i++;
		}
		else
			i++;
		j++;
	}
	v[j] = '\0';
	return (v);
}

char	*get_varvalue_first(t_ms *ms, char *arg, int i, int j)
{
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
	int		i;
	int		j;
	char	*exp_arg;
	char	*var;

	i = find_cur_dol(arg, data);
	if (!arg[i] || !arg[i + 1] || dol_standalone(&arg[i + 1], dol))
		return (arg);
	if (arg[i + 1] == '$')
		arg = keep_one_dol_only(ms, arg, i, dol);
	if (!arg)
		return (NULL);
	j = delimitate_var(arg, i, dol);
	var = get_varvalue_first(ms, arg, i, j);
	if (!var && ms->exit_code == 255)
		return (free(arg), NULL);
	else if (!var)
		exp_arg = skip_dol(arg, i, j, data);
	else
		exp_arg = repl_dol(arg, var, i, j);
	if (!exp_arg)
		return (ms->exit_code = 255, free_if(var), free(arg), NULL);
	if (var)
		data[2] += ft_strlen(var);
	return (data[0] -= 1, free_if(var), free(arg), exp_arg);
}
