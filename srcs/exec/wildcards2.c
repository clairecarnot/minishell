#include "../../include/exec.h"

int	same_as_wildcard(char *d_name, char *arg, int *flag)
{
	if ((*d_name == '\0' && *arg == '\0') || \
	(*arg == '*' && *flag == 0 && *(arg + 1) == '\0'))
		return (1);
	if (*arg == '\0' || *d_name == '\0')
		return (0);
	if (*arg == '*' && *flag == 0)
	{
		while (*(arg + 1) == '*' && *(flag + 1) == 0)
		{
			arg++;
			flag++;
		}
		if (!same_as_wildcard(d_name, arg + 1, flag + 1))
			return (same_as_wildcard(d_name + 1, arg, flag));
		else
			return (1);
	}
	if (*d_name == *arg)
		return (same_as_wildcard(d_name + 1, arg + 1, flag + 1));
	return (0);
}

char	**wldc2_split(char *new_arg)
{
	char	**split_arg;

	split_arg = NULL;
	if (new_arg)
	{
		split_arg = ft_split(new_arg, '\v');
		free(new_arg);
		if (!split_arg)
			return (NULL);// c'est verifie
		return (split_arg);
	}
	split_arg = ft_calloc(sizeof(char *), 1 + 1);
	if (!split_arg)
		return (free(new_arg), NULL);// c'est verifie
	split_arg[0] = ft_calloc(sizeof(char), 1 + 1);
	if (!split_arg[0])
		return (free(new_arg), free_tab(split_arg), NULL);// c'est verifie
	split_arg[0][1] = '\0';
	split_arg[1] = 0;
	return (split_arg);
}

char	*wldc2_join(char *d_name, char *new_arg)
{
	char	*tmp_arg;
	char	*new_arg2;

	tmp_arg = NULL;
	new_arg2 = NULL;
	tmp_arg = ft_strjoin(new_arg, "\v");
	free(new_arg);
	new_arg = NULL;
	if (!tmp_arg)
		return (free(new_arg), NULL);// c'est verifie 
	new_arg2 = ft_strjoin(tmp_arg, d_name);
	free(tmp_arg);
	if (!new_arg2)
		return (free(new_arg), NULL);// c'est verifie
	return (new_arg2);
}

char	*wildc2_dup_join(char *d_name, t_wildcard *wc, char *new_arg)
{
	if (wc->j == 0)
	{
		new_arg = ft_strdup(d_name);
		if (!new_arg)
			return (NULL);// c'est verifie
	}
	else
	{
		new_arg = wldc2_join(d_name, new_arg);
		if (!new_arg)
			return (NULL); // c'est verifie
	}
	wc->j++;
	return (new_arg);
}

char	**wildcards_2(DIR *d, struct dirent *file, char **args, t_wildcard *wc)
{
	char	*new_arg;

	new_arg = NULL;
	wc->j = 0;
	while (file != NULL)
	{
		if (same_as_wildcard(file->d_name, args[wc->i], wc->tab_w) == 1)
		{
			if (file->d_name[0] != '.' || (ft_strncmp("ls", args[0], 3) == 0 \
			&& ft_strncmp("-a", args[1], 3) == 0 && file->d_type != 4) \
			|| (args[wc->i][0] == '.' && (args[wc->i][1] == '*')))
			{
				new_arg = wildc2_dup_join(file->d_name, wc, new_arg);
				if (!new_arg)
					return (NULL);
			}
		}
		file = readdir(d);
	}
	return (wldc2_split(new_arg));
}
