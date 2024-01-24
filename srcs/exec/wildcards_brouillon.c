char	*wldc2_join(char *d_name, char *new_arg, t_wildcard *wildc)
{
	char	*new_arg2;

	new_arg2 = NULL;
	wildc->tmp_arg = ft_strjoin(new_arg, wildc->join);
	free(new_arg);
	new_arg = NULL;
	if (!wildc->tmp_arg)
		return (NULL);// a verifer 
	new_arg2 = ft_strjoin(wildc->tmp_arg, d_name);
	free(wildc->tmp_arg);
	if (!new_arg2)
		return (NULL);// a verifier
	return (new_arg2);
}

char	**wildcards_2(DIR *d, struct dirent *file, char **args, t_wildcard *wildc)
{
	char	*new_arg;

	new_arg = NULL;
	while (file != NULL)
	{
		if (same_as_wildcard(file->d_name, args[wildc->i], wildc->tab_w) == 1)
		{
			if (file->d_name[0] != '.' || (ft_strncmp("ls", args[0], 3) == 0 \
			&& ft_strncmp("-a", args[1], 3) == 0 && file->d_type != 4) \
			|| (args[wildc->i][0] == '.' && (args[wildc->i][1] == '*')))
			{
				if (++(wildc->j) == 0)
					if (!(new_arg = ft_strdup(file->d_name)))
						return (NULL);// a verifier
				if (wildc->j > 0)
					if (!(new_arg = wldc2_join(file->d_name, new_arg, wildc)))
						return (NULL); // a verfier 
			}
		}
		file = readdir(d);
	}
	// return (wldc2_split(new_arg, wildc));
	if (new_arg)
	{
		dprintf(2, "new_arg : %s\n", new_arg);
		wildc->split_arg = ft_split(new_arg, '\v');// a proteger
		free(new_arg);
		if (!wildc->split_arg)
		{
			// free(wildc->new_arg);
			return (NULL);
		}
		return (wildc->split_arg);
	}
	else
	{
		wildc->split_arg = ft_calloc(sizeof(char*), 1 + 1);
		wildc->split_arg[0] = ft_calloc(sizeof(char), 1 + 1);
		wildc->split_arg[0][1] = '\0';
		wildc->split_arg[1] = 0;
		if (!wildc->split_arg)// a verifier
			return (NULL);
		return (wildc->split_arg);
	}
}