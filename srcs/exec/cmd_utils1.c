#include "../../include/exec.h"

int	equals(char *s1, char *s2)
{
	if (!s1 || !s2)
		return (0);
	if (ft_strlen(s1) != ft_strlen(s2))
		return (0);
	if (ft_strncmp(s1, s2, ft_strlen(s1)) == 0)
		return (1);
	return (0);
}

t_builtin_type	builtin_type_is(char *builtin)
{
	if (equals(builtin, "echo"))
		return (ECHO);
	if (equals(builtin, "cd"))
		return (CD);
	if (equals(builtin, "pwd"))
		return (PWD);
	if (equals(builtin, "export"))
		return (EXPORT);
	if (equals(builtin, "unset"))
		return (UNSET);
	if (equals(builtin, "env"))
		return (ENV);
	if (equals(builtin, "exit"))
		return (EXIT);
	return (NOBUILT);
}

char	**lst_to_tab(t_list *lst)
{
	int		i;
	char	**args;
	t_list	*tmp;

	i = 0;
	tmp = lst;
	args = ft_calloc(ft_lstsize(lst) + 1, sizeof(char *));
	if (!args)
		return (NULL);
	while (tmp)
	{
		if (tmp->content)
		{
			args[i] = ft_strdup(tmp->content);
			if (!args[i])
				return (free_tab(args), NULL);
		}
		else
			args[i] = NULL;
		i++;
		tmp = tmp->next;
	}
	args[i] = 0;
	return (args);
}

char	*ft_strjoin_slash(char const *s1, char const *s2)
{
	char	*dest;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (0x0);
	dest = malloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 2);
	if (!dest)
		return (0x0);
	i = 0;
	while (((char *)s1)[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '/';
	i++;
	j = -1;
	while (((char *)s2)[++j])
		dest[i + j] = s2[j];
	dest[i + j] = '\0';
	return (dest);
}

char	*ft_slash_addback(char const *s1)
{
	char	*dest;
	int		i;

	if (!s1)
		return (0x0);
	dest = malloc(sizeof(char) * ft_strlen(s1) + 2);
	if (!dest)
		return (0x0);
	i = 0;
	while (((char *)s1)[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '/';
	i++;
	dest[i] = '\0';
	return (dest);
}
