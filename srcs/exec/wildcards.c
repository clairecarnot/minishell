#include "../../include/exec.h"

	// int		j;

	// while (args[ms->i_w])
	// {
	// 	j = 0;
	// 	while (args[ms->i_w][j])
	// 	{
			
			
	// 		j++;
	// 	}
	// 	ms->i++;
	// }
	// return (args);
int	asterisk(char *file, char *exp)
{
	if (!ft_strcmp(exp, "*") || !ft_strcmp(file, exp))
		return (1);
	if (!*file)
		return (0);
	if (*exp == '*')
		return (asterisk(file, &(exp[1])) || asterisk(&(file[1]), exp));
	if (*file == *exp)
		return (asterisk(&(file[1]), &(exp[1])));
	return (0);
}

char	**replace_asterisk(t_ms *ms, char **args)
{

	(void)ms;
	(void)args;
	int				j;
	int				k;
	DIR				*d;
	struct dirent	*entry;

	j = 0;
	k = 0;
	d = opendir(".");
	if (!d)
	{
		perror("Erreur lors de l'ouverture du répertoire");
		return (NULL);
	}
	entry = readdir(d);
	while (entry)
	{
        // On a un nom de fichier du repertoire + char * de l'args qu'il faut analyser
		// blabla.c include src main.c
        
		
		// while (args[ms->i_w][j])
		// {
		// 	while (args[ms->i_w][j] && args[ms->i_w][j] != '*')
		// 	{
				
		// 		j++
		// 	}
		// 	j++;
		// }

		entry = readdir(d);
    }
	closedir(d);
	return (args);
}

int	has_asterisk(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

int	cmd_wildcard(t_ms *ms ,t_cmd* cmd, t_wil *wil)
{
	// int	j;
	(void)wil;
	while (cmd->args[ms->i_w])
	{
		if (has_asterisk(cmd->args[ms->i_w]) == 1)
		{
			cmd->args = replace_asterisk(ms, cmd->args);
		}
		ms->i_w++;
	}
	return (0);
}

