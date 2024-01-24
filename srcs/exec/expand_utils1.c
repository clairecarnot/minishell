#include "../../include/exec.h" 

int	is_same_len(char *s1, char *s2)
{
	int	len;
	int	i;

	i = 0;
	len = ft_strlen(s1);
	while (s2[i] && s2[i] != '=')
		i++;
	if (i == len)
		return (1);
	return (0);
}

int	isset(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	count_consec_spc(char *value)
{
	int	i;
	int	j;
	int	consec_spc;

	i = 0;
	consec_spc = 0;
	while (value[i])
	{
		j = 1;
		if (value[i] == ' ')
		{
			while (value[i + j] && value[i + j] == ' ')
			{
				consec_spc++;
				j++;
			}
		}
		i += j;
	}
	return (consec_spc);
}

int	find_cur_dol(char *arg, int data[5])
{
	int	i;
	int	dol_nb;

	i = 0;
	dol_nb = data[0];
	while (arg[i] && (arg[i] != '$' || (arg[i] == '$' && dol_nb > 0)))
	{
		if (arg[i] == '$')
			dol_nb--;
		i++;
	}
	return (i);
}
