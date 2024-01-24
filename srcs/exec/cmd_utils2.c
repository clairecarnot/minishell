#include "../../include/exec.h"
#include "../../include/signals.h"

void	save_ptrs(t_list **s1, t_list **s2, t_list **t1, t_list **t2)
{
	*t1 = *s1;
	*t2 = *s2;
}

char	**tab_cpy(t_ms *ms, char **tab)
{
	int		i;
	int		size;
	char	**cpy;

	size = 0;
	while (tab[size])
		size++;
	cpy = ft_calloc(size + 1, sizeof(char *));
	if (!cpy)
	{
		ms->exit_code = 134;
		return (NULL);
	}
	i = 0;
	while (i < size)
	{
		cpy[i] = ft_strdup(tab[i]);
		if (!cpy[i])
			return (free_tab(cpy), NULL);
		i++;
	}
	cpy[i] = NULL;
	return (cpy);
}
