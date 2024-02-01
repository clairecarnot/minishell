/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_getvar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:23:47 by ccarnot           #+#    #+#             */
/*   Updated: 2024/02/01 17:23:48 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h" 

char	*ft_getenv(t_ms *ms, char *var)
{
	t_list	*tmp;
	char	*value;

	value = NULL;
	tmp = ms->env;
	while (tmp)
	{
		if (ft_strncmp(var, tmp->content, ft_strlen(var)) == 0
			&& is_same_len(var, tmp->content))
		{
			value = ft_strdup(tmp->content + ft_strlen(var) + 1);
			if (!value)
			{
				ms->exit_code = 255;
				return (NULL);
			}
			return (value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char	*exp_exitcode(t_ms *ms)
{
	char	*new_var;

	new_var = ft_itoa(ms->previous_exit_code);
	if (!new_var)
	{
		ms->exit_code = 255;
		return (NULL);
	}
	return (new_var);
}

char	*trim_beg(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		k;
	char	*d;

	i = 0;
	if (!s1 || !set)
		return (0x0);
	while (s1[i] && isset(s1[i], set))
		i++;
	j = ft_strlen(s1) - 1;
	d = malloc(sizeof(char) * (j - i + 2));
	if (!d)
		return (0x0);
	k = 0;
	while (i <= j)
	{
		d[k] = s1[i];
		k++;
		i++;
	}
	d[k] = '\0';
	return (d);
}

char	*ft_trimvar(char *value)
{
	char	*trim_val;
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
	trim_val = trim_beg(v, " ");
	return (free(v), trim_val);
}

char	*get_varvalue(t_ms *ms, char *arg, int i, int j)
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
	new_var = ft_trimvar(value);
	if (!new_var)
	{
		ms->exit_code = 255;
		return (free(value), free(var), NULL);
	}
	return (free(value), free(var), new_var);
}
